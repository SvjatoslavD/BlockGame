//
// Created by svjat on 7/27/2025.
//

#include "Chunk.h"
#include "World.h"

#include <utility>
#include <vector>

Chunk::Chunk(ChunkData& data, World& world) :
	chunk_data_(data), world_(&world) {}

Chunk::~Chunk() {
    chunk_data_.cube_data.clear();
}

void Chunk::RenderChunk() {
	if (chunk_mesh_.mesh_data_generated_ == false) {
		chunk_mesh_.GenerateMeshData(chunk_data_.chunk_coords, world_,chunk_data_.cube_data);
	}
	chunk_mesh_.RenderOpaque();
}

int Chunk::CalculateIndex(const int x, const int y, const int z) const {
    return (int)(x * k_chunk_size_ * k_chunk_size_) + (z * k_chunk_size_ ) + (y);
}

std::vector<CubeData>& Chunk::getCubeData() {
    return chunk_data_.cube_data;
}
