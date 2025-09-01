//
// Created by svjat on 7/27/2025.
//

#ifndef CHUNK_H
#define CHUNK_H

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <utility>
#include <vector>

#include "ChunkMesh.h"

struct ChunkMetaData {
	unsigned int air_count = 0;
	unsigned int solid_block_count = 0;

	bool is_all_air = false;
	bool is_all_blocks = false;
	bool has_opaque_blocks = false;
	bool has_transparent_blocks = false;

	void CalculateFlags() {
		if (air_count == 32 * 32 * 32) { is_all_air = true; }
		if (solid_block_count == 32 * 32 * 32) { is_all_blocks = true; }
		if (solid_block_count > 0) { has_opaque_blocks = true; }
	}
};

struct ChunkData {
	glm::ivec3 chunk_coords;
	ChunkMetaData meta_data;
	std::vector<CubeData> cube_data;
};

class World;

class Chunk {
public:
    Chunk(ChunkData& data, World& world);
    ~Chunk();

    std::vector<CubeData>& getCubeData();

    void RenderChunk();
	void setChunkData(ChunkData& data) {chunk_data_ = data;}
	glm::ivec3 getChunkCoords() const { return chunk_data_.chunk_coords; };

private:
    const unsigned int k_chunk_size_ = 32;

	ChunkData chunk_data_;

    int CalculateIndex(int x, int y, int z) const;

	ChunkMesh chunk_mesh_;
	World* world_;
};
#endif // CHUNK_H
