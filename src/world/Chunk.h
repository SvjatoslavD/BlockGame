//
// Created by svjat on 7/27/2025.
//

#ifndef CHUNK_H
#define CHUNK_H

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <vector>

#include "opengl/EBO.h"
#include "opengl/VAO.h"
#include "opengl/VBO.h"

class World;

struct CubeData;
enum SolidBlockType;

class Chunk {
public:
    Chunk(std::vector<CubeData> cube_data, World& world,glm::ivec3 chunk_coords);
    ~Chunk();

    std::vector<CubeData>& getCubeData();
	bool getMeshCreated() const {return mesh_created_;}
	void setMeshCreated(bool mesh_created) {mesh_created_ = mesh_created;}

	void GenerateFaces(World& world);
    void RenderChunk();

private:
    VAO VAO1_;
    VBO VBO1_;
    EBO EBO1_;

    glm::ivec3 chunk_coords_;

    const unsigned int k_chunk_size_ = 32;
    int indices_size = 0;
    bool mesh_created_ = false;

    std::vector<CubeData> cube_data_;
    std::vector<unsigned int> indices;
    std::vector<Vertex> vertices;

    void BindVAOAttributes();
    std::vector<unsigned int> AddIndices(unsigned int face_count);
    int CalculateIndex(int x, int y, int z) const;
};

struct CubeData {
    bool is_air;
    glm::vec3 position;
    SolidBlockType type;
};
#endif // CHUNK_H
