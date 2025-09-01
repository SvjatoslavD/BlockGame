//
// Created by svjat on 8/1/2025.
//

#ifndef WORLD_H
#define WORLD_H

#include <map>

#include "ThreadManager.h"
#include "rendering/Camera.h"
#include "world/Chunk.h"
#include "world/WorldGeneration.h"

struct CubeData;
class ThreadManager;

struct Vec3Less {
	bool operator()(const glm::vec3& a, const glm::vec3& b) const {
		if (a.x != b.x) return a.x < b.x;
		if (a.y != b.y) return a.y < b.y;
		return a.z < b.z;
	}
};

class World {
public:
	World() = default;
	~World();

	void Setup(int seed, ThreadManager* thread_manager);
	bool ChunkExists(glm::ivec3 key);
	std::vector<CubeData>& getChunkData(glm::ivec3 key);
	void Update(glm::ivec3 player_chunk_coords);
	void RenderChunks(Shader& shader, Camera& camera);

	WorldGeneration& getWorldGeneration() {return world_generation_;};

private:
	int seed_;
	int render_distance_ = 12;
	bool pause_chunk_loading = true;

	unsigned int k_chunk_size_x_ = 32;
	unsigned int k_chunk_size_y_ = 32;
	unsigned int k_chunk_size_z_ = 32;

	glm::ivec3 center_chunk_coords = glm::ivec3(0, 0, 0);

	std::map<glm::ivec3, std::unique_ptr<Chunk>, Vec3Less> chunks_;

	void GenerateChunk(glm::ivec3 key);

	WorldGeneration world_generation_;
	ThreadManager* thread_manager_;
};


#endif //WORLD_H
