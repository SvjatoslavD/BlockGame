//
// Created by svjat on 8/1/2025.
//

#include "World.h"
#include "Chunk.h"

void World::Setup(int seed) {
	seed_ = seed;
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

// Multiple noise layers for realistic terrain
    FastNoiseLite continent_noise, mountain_noise, hill_noise;

    // Large scale continent shapes
    continent_noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2S);
    continent_noise.SetFrequency(0.003f);
    continent_noise.SetSeed(seed_);

    // Mountain ridges
    mountain_noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
	mountain_noise.SetFractalType(FastNoiseLite::FractalType_Ridged);
	mountain_noise.SetFractalOctaves(1);
    mountain_noise.SetFrequency(0.001f);
    mountain_noise.SetSeed(seed_);

    // Rolling hills
    hill_noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    hill_noise.SetFrequency(0.02f);
    hill_noise.SetSeed(seed_);

    glm::fvec2 chunk_coords = glm::fvec2(key);

    for (int x = 0; x < k_chunk_size_x_; x++) {
        for (int z = 0; z < k_chunk_size_z_; z++) {
            float world_x = chunk_coords.x * 16 + x;
            float world_z = chunk_coords.y * 16 + z;

            // Combine multiple noise octaves
            float continent = continent_noise.GetNoise(world_x, world_z) * 0.3f;
            float mountain = mountain_noise.GetNoise(world_x, world_z) * 0.5f;
            float hill = hill_noise.GetNoise(world_x, world_z) * 0.15f;

            // Combine all noise layers
            float combined_noise = continent + mountain + hill;

            // More natural height mapping
            int base_height = 32; // Sea level
            int height_variation = 80; // Max height above sea level
        	int mountain_height = base_height + height_variation/6;
            int terrain_height = base_height + static_cast<int>(combined_noise * height_variation);

            // Clamp to chunk bounds
            terrain_height = std::max(0, std::min(terrain_height,(int)(k_chunk_size_y_ - 1)));

            for (int y = 0; y < k_chunk_size_y_; y++) {
                CubeData cube_data{};
                cube_data.position = glm::vec3(x, y, z);

                if (y <= terrain_height) {
                    cube_data.is_air = false;

                    // Better layer system
                	if (y >= mountain_height) { cube_data.type = STONE_BLOCK;}
                	else {
                		if (y == terrain_height) { cube_data.type = GRASS_BLOCK;}
                		else if (y >= terrain_height - 3) { cube_data.type = DIRT_BLOCK; }
                		else { cube_data.type = STONE_BLOCK; }
                	}
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

