//
// Created by svjat on 8/1/2025.
//

#ifndef WORLD_H
#define WORLD_H

#include <unordered_map>

#include "Camera.h"
#include "Chunk.h"

struct IVec2Hash;

class World {
public:
	World();
	~World();

	void Update(glm::vec3 cam_position);
	void RenderChunks(Shader& shader, Camera& camera);

private:
	unsigned int k_chunk_size_x_ = 16;
	unsigned int k_chunk_size_y_ = 512;
	unsigned int k_chunk_size_z_ = 16;

	std::vector<CubeData> GenerateChunkData();
	// the position of the chunk is written in x,z coordinates
	std::unordered_map<std::string, std::unique_ptr<Chunk>> chunks_;
	int render_distance_ = 2;
};
#endif //WORLD_H
