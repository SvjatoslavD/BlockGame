//
// Created by svjat on 8/1/2025.
//

#ifndef WORLD_H
#define WORLD_H

#include <map>

#include "Camera.h"
#include "Chunk.h"

class World {
public:
	World();
	~World();

	void Update(glm::vec2 player_chunk_coords);
	void RenderChunks(Shader& shader, Camera& camera);

private:
	int render_distance_ = 1;

	unsigned int k_chunk_size_x_ = 16;
	unsigned int k_chunk_size_y_ = 512;
	unsigned int k_chunk_size_z_ = 16;

	glm::vec2 center_chunk_coords = glm::vec2(0, 0);

	// the position of the chunk is written in x,z coordinates
	std::map<std::string, std::unique_ptr<Chunk>> chunks_;
	std::vector<std::string> rendered_chunks_;

	std::vector<CubeData> GenerateChunkData();
	void GenerateChunk(std::string key);

};
#endif //WORLD_H
