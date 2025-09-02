//
// Created by svjat on 8/1/2025.
//

#include "world/World.h"
#include "world/WorldGeneration.h"

#include <map>

#include "Chunk.h"

void World::Setup(int seed, ThreadManager* thread_manager) {
	seed_ = seed;
	thread_manager_ = thread_manager;

	world_generation_.setSeed(seed_);

	// load starting chunks into queue
	for (int y = -4; y <= 8; y++) {
		for (int x = -render_distance_; x <= render_distance_; x++) {
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
		// deloads all chunks not in render distance
		std::vector<glm::ivec3> chunks_to_erase;

		for (auto& chunk : chunks_) {
			if (abs(chunk.first.x - center_chunk_coords.x) > render_distance_
				|| abs(chunk.first.y - center_chunk_coords.y) > render_distance_
				|| abs(chunk.first.z - center_chunk_coords.z) > render_distance_) {
				chunks_to_erase.push_back(chunk.first);
			}
		}

		for (auto& chunk : chunks_to_erase) {
			chunks_.erase(chunk);
		}

		// queue chunks which should be in render distance
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

	auto completed_chunks = thread_manager_->getCompletedChunks();
	for (auto& chunk : completed_chunks) {
		chunks_.emplace(chunk->getChunkCoords(), std::move(chunk));
	}
}

void World::RenderChunks(Shader& shader, Camera& camera) {
	for (auto& chunk : chunks_) {
		// Chunk coords are in x,z format
		glm::vec3 offset = glm::vec3(chunk.first.x * (int)k_chunk_size_x_, chunk.first.y * (int)k_chunk_size_y_, chunk.first.z * (int)k_chunk_size_z_);
		camera.ModelMatrix(shader, "modelMatrix", offset);
		chunk.second->RenderChunk();
	}
}

void World::GenerateChunk(glm::ivec3 key) {
	if (chunks_.find(key) == chunks_.end()) {
		thread_manager_->QueueChunkLoad(key, 1);
	}
}

bool World::ChunkExists(glm::ivec3 key) {
	return chunks_.find(key) != chunks_.end();
}

std::vector<CubeData>& World::getChunkData(glm::ivec3 key) {
	return chunks_.operator[](key)->getCubeData();
}

