//
// Created by svjat on 8/1/2025.
//

#include "world/World.h"

#include <map>

#include "Chunk.h"

void World::Setup(int seed) {
	seed_ = seed;

	// Large scale continent shapes
	continent_noise_.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
	continent_noise_.SetFrequency(0.002f);
	continent_noise_.SetSeed(seed_);

	// Erosion noise
	erosion_noise_.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2S);
	erosion_noise_.SetFrequency(0.002f);
	erosion_noise_.SetSeed(seed_);

	// Peaks and valley noise
	peak_and_valley_noise_.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2S);
	peak_and_valley_noise_.SetFrequency(0.004f);
	peak_and_valley_noise_.SetFractalType(FastNoiseLite::FractalType_Ridged);
	peak_and_valley_noise_.SetFractalOctaves(1);
	peak_and_valley_noise_.SetSeed(seed_);

	continent_spline_.insert({
		{-1,0},{-0.8,20},{-0.75,50},{-0.5,100},{-0.3,130},
		{0.1,160},{0.15,180},{0.5,240},{0.7,250},{0.8,270},{1,300}
	});
	erosion_spline_.insert({
		{-1,0.01},{-0.5,0.15},{-.3,0.25},{-.2,0.3},{-0.1,0.35},
		{0.1,0.4},{0.3,0.5},{0.6,0.75},{0.6,0.8},{0.8,0.9},{1,0.95}
	});
	peak_and_valley_spline_.insert({
		{-1,-160},{-0.8,-130},{-0.7,-140},{-0.1,-80},
		{0.1,-20},{0.3,30},{0.6,50},{0.9,90},{1,100}
	});

	// load starting chunks into queue
	for (int x = -render_distance_; x <= render_distance_; x++) {
		for (int y = -1; y <= render_distance_; y++) {
			for (int z = -render_distance_; z <= render_distance_; z++) {
				GenerateChunk(glm::ivec3(x,y,z));
			}
		}
	}
}

World::~World() {
}

void World::Update(glm::ivec3 player_chunk_coords) {
	if (player_chunk_coords != center_chunk_coords && !pause_chunk_loading) {
		glm::ivec3 difference = player_chunk_coords - center_chunk_coords;

		// deloads all chunks not in render distance
		std::vector<glm::ivec3> chunks_to_erase;

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
		std::vector<glm::ivec3> chunks_to_create;

		for (int x = -render_distance_ + center_chunk_coords.x; x <= render_distance_ + center_chunk_coords.x; x++) {
			for (int y = -render_distance_ + center_chunk_coords.y; y <= render_distance_ + center_chunk_coords.y; y++) {
				for (int z = -render_distance_ + center_chunk_coords.y; z <= render_distance_ + center_chunk_coords.y; z++) {
					if (!ChunkExists(glm::ivec3(x,y,z))) {
						chunks_to_create.push_back(glm::ivec3(x,y,z));
					}
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
		glm::vec3 offset = glm::vec3(chunk.first.x * (int)k_chunk_size_x_, chunk.first.y * (int)k_chunk_size_y_, chunk.first.z * (int)k_chunk_size_z_);
		camera.ModelMatrix(shader, "modelMatrix", offset);
		chunk.second->RenderChunk();
	}
}

std::vector<CubeData> World::GenerateChunkData(glm::ivec3 key) {
	std::vector<CubeData> cube_data_vec;
	cube_data_vec.reserve(k_chunk_size_x_*k_chunk_size_y_*k_chunk_size_z_);

    auto chunk_coords = glm::fvec3(key);
	int y_offset = (int)chunk_coords.y * (int)k_chunk_size_y_;

    for (int x = 0; x < k_chunk_size_x_; x++) {
    	for (int z = 0; z < k_chunk_size_z_; z++) {
    		float world_x = chunk_coords.x * k_chunk_size_x_ + x;
    		float world_z = chunk_coords.z * k_chunk_size_z_ + z;

    		// Combine multiple noise octaves
    		float continent = continent_noise_.GetNoise(world_x, world_z);
    		float erosion = erosion_noise_.GetNoise(world_x, world_z);
    		float peak_and_valley = peak_and_valley_noise_.GetNoise(world_x, world_z) * .5f;

    		const int continent_height = (int)getSplineValue(continent, CONTINENT, continent);
    		const float erosion_mult = getSplineValue(erosion, EROSION, erosion);
    		const int peak_and_valley_height = (int)getSplineValue(peak_and_valley, PEAK_AND_VALLEY, peak_and_valley);

    		// Combine all noise layers
    		int base_height = sea_level_ + (int)(continent_height * erosion_mult);
    		int additional_height = (int)(peak_and_valley_height * erosion_mult);
    		int combined_noise = base_height + additional_height;

    		for (int y = 0; y < k_chunk_size_y_; y++) {
    			int true_y = y + y_offset;
    			CubeData cube_data{};
    			cube_data.position = glm::vec3(x, y, z);

    			if (true_y <= combined_noise) {
    				if (continent_height > 190 && erosion_mult > 0.6 && peak_and_valley_height > -15 && true_y >= combined_noise - 4) {cube_data.type = SNOW_BLOCK; }
    				else if (continent_height > 110 && erosion_mult > .4) {cube_data.type = STONE_BLOCK; }
    				else {
    					if (true_y == combined_noise) {cube_data.type = GRASS_BLOCK;}
    					else if (true_y >= combined_noise - 3) {cube_data.type = DIRT_BLOCK;}
    					else {cube_data.type = STONE_BLOCK;}
    				}
    			}
    			else if (true_y > combined_noise && true_y < sea_level_) {
    				cube_data.type = SAND_BLOCK;
    			}
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

void World::GenerateChunk(glm::ivec3 key) {
	if (chunks_.find(key) == chunks_.end()) {
		std::vector<CubeData> data = GenerateChunkData(key);
		auto chunk = std::make_unique<Chunk>(data, *this, key);
		chunks_.emplace(key, std::move(chunk));
	}
}

bool World::ChunkExists(glm::ivec3 key) {
	return chunks_.find(key) != chunks_.end();
}

std::vector<CubeData>& World::getChunkData(glm::ivec3 key) {
	return chunks_.operator[](key)->getCubeData();
}

float World::getSplineValue(float noise, SplineType type, float height_noise) {
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
			interp = height_noise < 0 ? SMOOTH : LINEAR;
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
				case LINEAR:
					calculated_value = y0 + t * (y1 - y0);
				break;

				case SMOOTH:
					t = t * t * (3.0f - 2.0f * t);
				calculated_value = y0 + t * (y1 - y0);
				break;

				case CUBIC:
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

