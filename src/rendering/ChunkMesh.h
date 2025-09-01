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
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
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
    ~ChunkMesh() = default;

    // === Mesh Generation ===
    void GenerateMeshData(glm::ivec3 chunk_pos, World* world, std::vector<CubeData>& cubes);
    void Clear();

	int CalculateIndex(int x, int y, int z) const;
	void AddIndices();

    // === GPU Management ===
    void UploadToGPU();
    void FreeGPUResources();

    // === Rendering Interface ===
    void RenderOpaque();
    void RenderTransparent() const;

	bool mesh_data_generated_ = false;

private:
	int k_chunk_size_ = 32;

	MeshData mesh_data_;
	GLMesh gl_mesh_;

// 	MeshData opaque_mesh_;      // Solid blocks
// 	MeshData transparent_mesh_; // Glass, water, leaves
//
// 	glm::ivec2 chunk_position_;
// 	bool needs_upload_ = false;
// 	bool is_empty_ = true;
//
//
//     // === Internal Mesh Building ===
//     void AddBlockFace(MeshData& mesh, BlockType block_type, FaceDirection face,
//                      glm::vec3 position, float light_level);
//
//     bool ShouldDrawFace(const Chunk& chunk, const std::array<const Chunk*, 6>& neighbors,
//                        int x, int y, int z, FaceDirection face) const;
//
//     void UploadMeshData(const MeshData& mesh_data, GLMesh& gl_mesh);
//
//     // === Face Generation Helpers ===
//     std::array<glm::vec3, 4> GetFaceVertices(FaceDirection face, glm::vec3 block_pos) const;
//     std::array<glm::vec2, 4> GetFaceUVs(BlockType block_type, FaceDirection face) const;
//     glm::vec3 GetFaceNormal(FaceDirection face) const;
};



#endif //CHUNKMESH_H
