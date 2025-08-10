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

struct Vec2Less {
	bool operator()(const glm::ivec2& a, const glm::ivec2& b) const {
		if (a.x < b.x) return true;
		if (a.x > b.x) return false;
		return a.y < b.y;
	}
};

class World {
public:
	World();
	~World();

	bool ChunkExists(glm::ivec2 key);
	std::vector<CubeData>& getChunkData(glm::ivec2 key);
	void Update(glm::ivec2 player_chunk_coords);
	void RenderChunks(Shader& shader, Camera& camera);

private:
	int render_distance_ = 5;
	bool pause_chunk_loading = false;

	unsigned int k_chunk_size_x_ = 16;
	unsigned int k_chunk_size_y_ = 512;
	unsigned int k_chunk_size_z_ = 16;

	glm::ivec2 center_chunk_coords = glm::ivec2(0, 0);

	// the position of the chunk is written in x,z coordinates
	std::map<glm::ivec2, std::unique_ptr<Chunk>, Vec2Less> chunks_;

	std::vector<CubeData> GenerateChunkData(glm::ivec2 key);
	void GenerateChunk(glm::ivec2 key);

};



#endif //WORLD_H
