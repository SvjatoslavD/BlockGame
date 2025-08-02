//
// Created by svjat on 8/1/2025.
//

#include "../header/World.h"
#include "../header/Chunk.h"

World::World() {
	// load starting chunks into queue
	for (int x = -render_distance_; x <= render_distance_; x++) {
		for (int z = -render_distance_; z <= render_distance_; z++) {
			std::string key = std::to_string(x) + "," + std::to_string(z);
			GenerateChunk(key);
		}
	}
}

World::~World() {

}

void World::Update(glm::vec2 player_chunk_coords) {
	if (player_chunk_coords != center_chunk_coords && !pause_chunk_loading) {
		glm::ivec2 difference = player_chunk_coords - center_chunk_coords;

		// player z changed
		if (difference.y != 0) {
			// loads chunks
			std::string key_z_value = (difference.y > 0)
			? std::to_string((int)(center_chunk_coords.y + render_distance_ + 1))
			: std::to_string((int)(center_chunk_coords.y - render_distance_ - 1));

			for (int x = -render_distance_; x <= render_distance_; x++) {
				std::string key = std::to_string((int)(x + center_chunk_coords.x)) + "," + key_z_value;
				GenerateChunk(key);
			}
		}

		// player x changed
		if (difference.x != 0) {
			// loads chunks
			std::string key_x_value = (difference.x > 0)
			? std::to_string((int)(center_chunk_coords.x + render_distance_ + 1))
			: std::to_string((int)(center_chunk_coords.x - render_distance_ - 1));

			for (int z = -render_distance_; z <= render_distance_; z++) {
				std::string key = key_x_value + "," + std::to_string((int)z+center_chunk_coords.y);
				GenerateChunk(key);
			}
		}

		// deloads all chunks not in render distance
		std::vector<std::string> chunks_to_erase;

		for (auto& chunk : chunks_) {
			int x = 0;
			int z = 0;
			char comma;
			std::stringstream ss(chunk.first);
			ss >> x >> comma >> z;

			if (abs(x - center_chunk_coords.x) > render_distance_ + abs(difference.x) || abs(z - center_chunk_coords.y) > render_distance_ + abs(difference.y)) {
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
	int x = 0;
	int z = 0;
	char comma;

	for (auto& chunk : chunks_) {
		std::stringstream ss(chunk.first);
		ss >> x >> comma >> z;
		// Chunk coords are in x,z format
		glm::vec3 offset = glm::vec3(x * (int)k_chunk_size_x_, 0, z * (int)k_chunk_size_z_);
		camera.ModelMatrix(shader, "modelMatrix", offset);
		chunk.second->RenderChunk();
	}
}

std::vector<CubeData> World::GenerateChunkData(std::string key) {
	std::vector<CubeData> cube_data_vec;
	cube_data_vec.reserve(k_chunk_size_x_*k_chunk_size_y_*k_chunk_size_z_);

	FastNoiseLite terrain_noise;
	terrain_noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
	terrain_noise.SetFrequency(0.01f);
	terrain_noise.SetSeed(0);

	glm::vec2 chunk_coords = ReadStringKey(key);

	for (int x = 0; x < k_chunk_size_x_; x++) {
		for (int z = 0; z < k_chunk_size_z_; z++) {
			float terrain_noise_val = terrain_noise.GetNoise(chunk_coords.x * 16 + x, chunk_coords.y * 16 + z);
			int height = static_cast<int>((terrain_noise_val + 1.0f) * 0.5f * (k_chunk_size_y_/64) + k_chunk_size_y_/4);

			for (int y = 0; y < k_chunk_size_y_; y++) {
				CubeData cube_data{};
				if (y < height) {cube_data.is_air = false;}
				else {cube_data.is_air = true;}
				cube_data.position = glm::vec3(x, y, z);
				cube_data_vec.emplace_back(cube_data);
			}
		}
	}

	return cube_data_vec;
}

void World::GenerateChunk(std::string key) {
	if (chunks_.find(key) == chunks_.end()) {
		std::vector<CubeData> data = GenerateChunkData(key);
		auto chunk = std::make_unique<Chunk>(data, *this, ReadStringKey(key));
		chunks_.emplace(key, std::move(chunk));
	}
}

bool World::ChunkExists(std::string key) {
	return chunks_.find(key) != chunks_.end();
}

std::vector<CubeData>& World::getChunkData(std::string key) {
	return chunks_.operator[](key)->getCubeData();
}

glm::vec2 World::ReadStringKey(std::string key) {
	glm::vec2 out = glm::vec2(0);
	char comma;
	std::stringstream ss(key);
	ss >> out.x >> comma >> out.y;
	return out;
}

