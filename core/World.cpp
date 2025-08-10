//
// Created by svjat on 8/1/2025.
//

#include "World.h"
#include "Chunk.h"

World::World() {
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

		// player z changed
		if (difference.y != 0) {
			// loads chunks
			int key_z_value = (difference.y > 0)
			? (int)(center_chunk_coords.y + render_distance_ + 1)
			: (int)(center_chunk_coords.y - render_distance_ - 1);

			for (int x = -render_distance_; x <= render_distance_; x++) {
				glm::ivec2 key = glm::ivec2((x + center_chunk_coords.x), key_z_value);
				GenerateChunk(key);
			}
		}

		// player x changed
		if (difference.x != 0) {
			// loads chunks
			int key_x_value = (difference.x > 0)
			? ((int)(center_chunk_coords.x + render_distance_ + 1))
			: ((int)(center_chunk_coords.x - render_distance_ - 1));

			for (int z = -render_distance_; z <= render_distance_; z++) {
				glm::ivec2 key = glm::ivec2(key_x_value ,(int)(z+center_chunk_coords.y));
				GenerateChunk(key);
			}
		}

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

		center_chunk_coords = player_chunk_coords;
	}
}

void World::RenderChunks(Shader& shader, Camera& camera) {
	for (auto& chunk : chunks_) {
		// Chunk coords are in x,z format
		glm::vec3 offset = glm::vec3(chunk.first.x * (int)k_chunk_size_x_, 0, chunk.first.y * (int)k_chunk_size_z_);
		camera.ModelMatrix(shader, "modelMatrix", offset);
		chunk.second->RenderChunk();
	}
}

std::vector<CubeData> World::GenerateChunkData(glm::ivec2 key) {
	std::vector<CubeData> cube_data_vec;
	cube_data_vec.reserve(k_chunk_size_x_*k_chunk_size_y_*k_chunk_size_z_);

	FastNoiseLite terrain_noise;
	terrain_noise.SetNoiseType(FastNoiseLite::NoiseType_ValueCubic);
	terrain_noise.SetFrequency(0.01f);
	terrain_noise.SetSeed(0);

	FastNoiseLite mountain_noise;
	mountain_noise.SetFrequency(0.005f);
	mountain_noise.SetSeed(1);
	mountain_noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);

	glm::fvec2 chunk_coords = glm::fvec2(key);
	int y_height = 0;

	for (int x = 0; x < k_chunk_size_x_; x++) {
		for (int z = 0; z < k_chunk_size_z_; z++) {
			float terrain_noise_val = terrain_noise.GetNoise(chunk_coords.x * 16 + x, chunk_coords.y * 16 + z);
			float mountain_noise_val = mountain_noise.GetNoise(chunk_coords.x * 16 + x, chunk_coords.y * 16 + z);

			int terrain_height = static_cast<int>((terrain_noise_val + 1.0f) * 0.5f * (k_chunk_size_y_/16));
			int mountain_height = static_cast<int>(pow(((mountain_noise_val + 1.0f) * 0.5f),.1) * (k_chunk_size_y_/8));

			y_height = mountain_height + terrain_height + k_chunk_size_y_/8;
			for (int y = 0; y < k_chunk_size_y_; y++) {
				CubeData cube_data{};
				if (y < y_height) {cube_data.is_air = false;}
				else {cube_data.is_air = true;}
				cube_data.position = glm::vec3(x, y, z);
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

