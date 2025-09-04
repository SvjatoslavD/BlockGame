//
// Created by svjat on 8/29/2025.
//

#ifndef CHUNKMESH_H
#define CHUNKMESH_H

#include <gl/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include "Tilemap.h"

#include "opengl/VAO.h"
#include "opengl/VBO.h"
#include "opengl/EBO.h"

class Chunk;
class World;

struct MeshData {
	std::vector<Vertex> vertices {};
	std::vector<uint32_t> indices = {};
	unsigned int face_count = 0;
};

struct GLMesh {
	VAO vao;
	VBO vbo;
	EBO ebo;
	uint32_t index_count = 0;
	bool uploaded = false;
};

class ChunkMesh {
public:
	// === Construction ===
	ChunkMesh() = default;
	~ChunkMesh() {FreeGPUResources();};

	// === Mesh Generation ===
	void GenerateMeshData(std::vector<CubeData>& cubes);
	void Clear();

	int CalculateIndex(int x, int y, int z) const;
	void AddIndices();
	void CheckNearbyChunks(glm::ivec3 chunk_pos, World* world);

	// === GPU Management ===
	void UploadToGPU();
	void FreeGPUResources();

	// === Rendering Interface ===
	void RenderOpaque();
	void RenderTransparent() const;

	bool mesh_data_generated_ = true;

private:
	int k_chunk_size_ = 32;

	MeshData mesh_data_;
	GLMesh gl_mesh_;

	void AddMeshData(glm::u8 face, glm::u8vec3 position,glm::u8 texture,std::array<glm::u8,4> ambient_occlusion_level);
	std::array<glm::u8,4> CalculateAmbientOcclusionLevels(glm::u8 face, std::vector<CubeData>& cubes, glm::u8vec3 position);

	// 	MeshData opaque_mesh_;      // Solid blocks
	// 	MeshData transparent_mesh_; // Glass, water, leaves

	std::vector<CubeData>* chunk_bottom_ = nullptr;
	std::vector<CubeData>* chunk_top_ = nullptr;
	std::vector<CubeData>* chunk_back_ = nullptr;
	std::vector<CubeData>* chunk_front_ = nullptr;
	std::vector<CubeData>* chunk_left_ = nullptr;
	std::vector<CubeData>* chunk_right_ = nullptr;
	bool found_all_neighbors_ = false;
};



#endif //CHUNKMESH_H
