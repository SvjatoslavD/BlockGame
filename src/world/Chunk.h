//
// Created by svjat on 7/27/2025.
//

#ifndef CHUNK_H
#define CHUNK_H

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <vector>

#include "ChunkMesh.h"
#include "opengl/EBO.h"
#include "opengl/VAO.h"
#include "opengl/VBO.h"

class World;

class Chunk {
public:
	struct ChunkMetaData {
		bool is_all_air = false;
		bool is_all_blocks = false;
		bool has_opaque_blocks = false;
		bool has_transparent_blocks = false;

	};

    Chunk(std::vector<CubeData> cube_data,glm::ivec3 chunk_coords, World& world);
    ~Chunk();

    std::vector<CubeData>& getCubeData();

    void RenderChunk();

private:
    glm::ivec3 chunk_coords_;

    const unsigned int k_chunk_size_ = 32;
    int indices_size = 0;

    std::vector<CubeData> cube_data_;

    int CalculateIndex(int x, int y, int z) const;

	ChunkMesh chunk_mesh_;
};
#endif // CHUNK_H
