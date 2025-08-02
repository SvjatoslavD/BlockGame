//
// Created by svjat on 8/1/2025.
//

#ifndef WORLD_H
#define WORLD_H

#include <map>

#include "../external/FastNoiseLite.h"
#include "Camera.h"

struct CubeData;
class Chunk;

class World {
public:
	World();
	~World();

	bool ChunkExists(std::string key);
	std::vector<CubeData>& getChunkData(std::string key);
	void Update(glm::vec2 player_chunk_coords);
	void RenderChunks(Shader& shader, Camera& camera);
	glm::vec2 ReadStringKey(std::string key);

private:
	int render_distance_ = 5;
	bool pause_chunk_loading = false;

	unsigned int k_chunk_size_x_ = 16;
	unsigned int k_chunk_size_y_ = 512;
	unsigned int k_chunk_size_z_ = 16;

	glm::vec2 center_chunk_coords = glm::vec2(0, 0);

	// the position of the chunk is written in x,z coordinates
	std::map<std::string, std::unique_ptr<Chunk>> chunks_;

	std::vector<CubeData> GenerateChunkData(std::string key);
	void GenerateChunk(std::string key);

};
#endif //WORLD_H
