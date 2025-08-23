//
// Created by svjat on 8/1/2025.
//

#include "World.h"
#include "Chunk.h"

void World::Setup(int seed) {
	seed_ = seed;

	// Large scale continent shapes
	continent_noise_.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2S);
	continent_noise_.SetFrequency(0.002f);
	continent_noise_.SetSeed(seed_);

	// Erosion noise
	erosion_noise_.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
	erosion_noise_.SetFrequency(0.008f);
	erosion_noise_.SetSeed(seed_);

	// Peaks and valley noise
	peak_and_valley_noise_.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
	peak_and_valley_noise_.SetFrequency(0.02f);
	peak_and_valley_noise_.SetFractalType(FastNoiseLite::FractalType_Ridged);
	peak_and_valley_noise_.SetFractalOctaves(1);
	peak_and_valley_noise_.SetFractalLacunarity(.3);
	peak_and_valley_noise_.SetFractalGain(.2);
	peak_and_valley_noise_.SetSeed(seed_);

	continent_spline_.insert({{-1,20},{-.8,0},{-.4,5},{-.1,50},{.1,55},{.3,80},{.6,120},{.8,130},{1,140}});
	erosion_spline_.insert({{-1,1.7},{-.7,1.55},{-.65,1.6},{-.2,1.0},{0.0,.4},{0.3,.2},{0.35,.4},{0.42,.4},{0.45,.2},{0.50,.1},{1,.1}});
	peak_and_valley_spline_.insert({{-1,-80},{-.8,-50},{-.7,-35},{-.2,-30},{.1,25},{.3,40},{.35,40},{.4,35},{.6,65},{.9,75},{1,80}});

	// load starting chunks into queue
	for (int x = -render_distance_; x <= render_distance_; x++) {
		for (int z = -render_distance_; z <= render_distance_; z++) {
			GenerateChunk(glm::ivec2(x,z));
		}
	}
}

World::~World() {

}

void World::Update(glm::ivec2 player_chunk_coords) {
	if (player_chunk_coords != center_chunk_coords && !pause_chunk_loading) {
		glm::ivec2 difference = player_chunk_coords - center_chunk_coords;

		// deloads all chunks not in render distance
		std::vector<glm::ivec2> chunks_to_erase;

		for (auto& chunk : chunks_) {
			if (abs(chunk.first.x - center_chunk_coords.x) > render_distance_ + abs(difference.x)
				|| abs(chunk.first.y - center_chunk_coords.y) > render_distance_ + abs(difference.y)) {
				chunks_to_erase.push_back(chunk.first);
			}
		}

		for (auto& chunk : chunks_to_erase) {
			chunks_.erase(chunk);
		}

		// loads chunks which should be in render distance
		std::vector<glm::ivec2> chunks_to_create;

		for (int x = -render_distance_ + center_chunk_coords.x; x <= render_distance_ + center_chunk_coords.x; x++) {
			for (int z = -render_distance_ + center_chunk_coords.y; z <= render_distance_ + center_chunk_coords.y; z++) {
				if (!ChunkExists(glm::ivec2(x,z))) {
					chunks_to_create.push_back(glm::ivec2(x,z));
				}
			}
		}

		for (auto chunk : chunks_to_create) {
			GenerateChunk(chunk);
		}

		center_chunk_coords = player_chunk_coords;
	}
}

void World::RenderChunks(Shader& shader, Camera& camera) {
	for (auto& chunk : chunks_) {
		// Chunk coords are in x,z format
		if (!chunk.second->getMeshCreated()) {
			chunk.second->GenerateFaces(*this);
			chunk.second->setMeshCreated(true);
		}
		glm::vec3 offset = glm::vec3(chunk.first.x * (int)k_chunk_size_x_, 0, chunk.first.y * (int)k_chunk_size_z_);
		camera.ModelMatrix(shader, "modelMatrix", offset);
		chunk.second->RenderChunk();
	}
}

std::vector<CubeData> World::GenerateChunkData(glm::ivec2 key) {
	std::vector<CubeData> cube_data_vec;
	cube_data_vec.reserve(k_chunk_size_x_*k_chunk_size_y_*k_chunk_size_z_);

    glm::fvec2 chunk_coords = glm::fvec2(key);

    for (int x = 0; x < k_chunk_size_x_; x++) {
        for (int z = 0; z < k_chunk_size_z_; z++) {
            float world_x = chunk_coords.x * 16 + x;
            float world_z = chunk_coords.y * 16 + z;

            // Combine multiple noise octaves
            float continent = continent_noise_.GetNoise(world_x, world_z);
            float erosion = erosion_noise_.GetNoise(world_x, world_z);
            float peak_and_valley = peak_and_valley_noise_.GetNoise(world_x, world_z) * .5f;

        	float continent_height = getSplineValue(continent, CONTINENT);
        	float erosion_mult = getSplineValue(erosion, EROSION);
        	float peak_and_valley_height = getSplineValue(peak_and_valley, PEAK_AND_VALLEY);

            // Combine all noise layers
            float combined_noise = sea_level_ + (int)((continent_height + peak_and_valley_height) * erosion_mult);

            for (int y = 0; y < k_chunk_size_y_; y++) {
                CubeData cube_data{};
                cube_data.position = glm::vec3(x, y, z);

                if (y <= combined_noise) {
                	if (peak_and_valley_height > 40) {cube_data.type = SNOW_BLOCK; }
                	else {
                		if (y == combined_noise) {cube_data.type = GRASS_BLOCK;}
                		else if (y >= combined_noise - 3) {cube_data.type = DIRT_BLOCK;}
                		else {cube_data.type = STONE_BLOCK;}
                	}
                }
            	else if (y > combined_noise && y < sea_level_) {cube_data.type = SAND_BLOCK;}
            	else {
                    cube_data.is_air = true;
                    cube_data.type = DIRT_BLOCK; // Air blocks
                }

                cube_data_vec.emplace_back(cube_data);
            }
        }
    }

	return cube_data_vec;
}

void World::GenerateChunk(glm::ivec2 key) {
	if (chunks_.find(key) == chunks_.end()) {
		std::vector<CubeData> data = GenerateChunkData(key);
		auto chunk = std::make_unique<Chunk>(data, *this, key);
		chunks_.emplace(key, std::move(chunk));
	}
}

bool World::ChunkExists(glm::ivec2 key) {
	return chunks_.find(key) != chunks_.end();
}

std::vector<CubeData>& World::getChunkData(glm::ivec2 key) {
	return chunks_.operator[](key)->getCubeData();
}

float World::getSplineValue(float noise, SplineType type) {
	std::map<float, float> spline_map;
	InterpolationType interp;

	switch (type) {
		case CONTINENT:
			spline_map = continent_spline_;
			interp = CUBIC;
		break;
		case EROSION:
			spline_map = erosion_spline_;
			interp = SMOOTH;
		break;
		case PEAK_AND_VALLEY:
			spline_map = peak_and_valley_spline_;
			interp = LINEAR;
		break;
	}

	auto first = spline_map.begin();
	auto last = spline_map.rbegin();

	// If noise is at or below the first point
	if (noise <= first->first) return first->second;
	// If noise is at or above the last point
	if (noise >= last->first) return last->second;

	for (auto it = spline_map.begin(); it != spline_map.end(); ++it) {
		if (noise == it->first) {
			return it->second;
		}
		if (noise < it->first) {
			// We found the upper bound, get the lower bound
			auto upper = it;
			auto lower = --it; // Move back to previous element

			// Get interpolation points
			float x0 = lower->first, y0 = lower->second;
			float x1 = upper->first, y1 = upper->second;
			float t = (noise - x0) / (x1 - x0);

			// Apply different interpolation methods
			float calculated_value;
			switch (interp) {
				case InterpolationType::LINEAR:
					calculated_value = y0 + t * (y1 - y0);
				break;

				case InterpolationType::SMOOTH:
					t = t * t * (3.0f - 2.0f * t);
				calculated_value = y0 + t * (y1 - y0);
				break;

				case InterpolationType::CUBIC:
					t = t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);
				calculated_value = y0 + t * (y1 - y0);
				break;

				default:
					calculated_value = y0 + t * (y1 - y0);
				break;
			}

			return calculated_value;
		}
	}

	// Should never reach here due to edge case handling above
	return 0.0f;
}

