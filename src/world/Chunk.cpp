//
// Created by svjat on 7/27/2025.
//

#include "Chunk.h"
#include "World.h"

#include <array>
#include <utility>
#include <vector>

Chunk::Chunk(std::vector<CubeData> cube_data, glm::ivec3 chunk_coords, World& world) :
	cube_data_(std::move(cube_data)), chunk_coords_(chunk_coords) {
	chunk_mesh_.GenerateMeshData(chunk_coords_, &world,cube_data_);
}

Chunk::~Chunk() {
    cube_data_.clear();
}

void Chunk::RenderChunk() {
	chunk_mesh_.RenderOpaque();
}

int Chunk::CalculateIndex(const int x, const int y, const int z) const {
    return (int)(x * k_chunk_size_ * k_chunk_size_) + (z * k_chunk_size_ ) + (y);
}

std::vector<CubeData>& Chunk::getCubeData() {
    return cube_data_;
}
