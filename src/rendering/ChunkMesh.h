//
// Created by svjat on 8/29/2025.
//

#ifndef CHUNKMESH_H
#define CHUNKMESH_H

#include <array>
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
	bool ids_created = false;
	bool uploaded = true;
};

class ChunkMesh {
public:
	// === Construction ===
	ChunkMesh() = default;
	~ChunkMesh() {FreeGPUResources();};



	// === Mesh Generation ===
	void GenerateMeshData(std::vector<CubeData>& cubes);
	void GenerateMainMesh( std::vector<CubeData>& cubes);
	void GenerateMeshCorners(std::vector<CubeData>& cubes);

	int CalculateIndex(int x, int y, int z) const;
	void AddIndices();
	void CheckNearbyChunks(glm::ivec3, World* world);
	CubeData GetNeighboringBlock(glm::i8vec3 block_position, std::vector<CubeData>& cubes);

	// === GPU Management ===
	void UploadToGPU();
	void FreeGPUResources();

	// === Rendering Interface ===
	void RenderOpaque();
	void RenderTransparent() const;

private:
	bool create_mesh = false;
	bool mesh_created = false;
	unsigned int vertex_index_ = 0;

	int k_chunk_size_ = 32;

	MeshData mesh_data_;
	GLMesh gl_mesh_;

	World* world_ = nullptr;
	glm::ivec3 chunk_position_;

	void AddMeshData(glm::u8 face, glm::u8vec3 position,glm::u8 texture,std::array<glm::u8,4> ambient_occlusion_level);
	std::array<glm::u8,4> CalculateAmbientOcclusionLevels(glm::u8 face, std::vector<CubeData>& cubes, glm::u8vec3 position);

	// 	MeshData opaque_mesh_;      // Solid blocks
	// 	MeshData transparent_mesh_; // Glass, water, leaves

	std::array<bool,6> neighbor_chunks_found_ = {false,false,false,false,false,false};
	std::array<glm::ivec3,6> neighbor_chunks_position_ = {
		glm::ivec3(0,-1,0),
		glm::ivec3(0,1,0),
		glm::ivec3(0,0,-1),
		glm::ivec3(0,0,1),
		glm::ivec3(-1,0,0),
		glm::ivec3(1,0,0)
	};
};



#endif //CHUNKMESH_H
