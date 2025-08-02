//
// Created by svjat on 8/1/2025.
//

#include "../header/World.h"

World::World() {
	// load starting chunks into queue
	for (int x = -render_distance_; x <= render_distance_; x++) {
		for (int z = -render_distance_; z <= render_distance_; z++) {
			std::string key = std::to_string(x) + "_" + std::to_string(z);
			GenerateChunk(key);
		}
	}
}

World::~World() {

}

void World::Update(glm::vec2 player_chunk_coords) {
	if (player_chunk_coords != center_chunk_coords) {
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

std::vector<CubeData> World::GenerateChunkData() {
	std::vector <CubeData> cube_data_vec;
	cube_data_vec.reserve(k_chunk_size_x_*k_chunk_size_y_*k_chunk_size_z_);

	for (int x = 0; x < k_chunk_size_x_; x++) {
		for (int y = 0; y < k_chunk_size_y_; y++) {
			for (int z = 0; z < k_chunk_size_z_; z++) {
				CubeData cube_data{};
				// cube_data.is_air = true;
				cube_data.is_air = false;
				// if ((x + y + z) % 2 == 0) { cube_data.is_air = true; }
				// else { cube_data.is_air = false; }
				cube_data.position = glm::vec3(x, y, z);
				cube_data_vec.emplace_back(cube_data);
			}
		}
	}

	return cube_data_vec;
}

void World::GenerateChunk(std::string key) {
	if (chunks_.find(key) == chunks_.end()) {
		std::vector<CubeData> data = GenerateChunkData();
		auto chunk = std::make_unique<Chunk>(data);
		chunks_.emplace(key, std::move(chunk));
	}
}

