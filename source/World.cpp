//
// Created by svjat on 8/1/2025.
//

#include "../header/World.h"

World::World() {
	for (int x = -render_distance_; x <= render_distance_; x++) {
		for (int z = -render_distance_; z <= render_distance_; z++) {
			std::vector<CubeData> data = GenerateChunkData();
			auto chunk = std::make_unique<Chunk>(data);
			chunks_.emplace(std::to_string(x) + "," + std::to_string(z), std::move(chunk));
		}
	}
}

World::~World() {

}

void World::Update(glm::vec3 cam_position) {

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

