//
// Created by svjat on 7/27/2025.
//

#ifndef CHUNK_H
#define CHUNK_H

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <vector>

#include "World.h"
#include "EBO.h"
#include "VAO.h"
#include "VBO.h"

class World;

struct CubeData;

class Chunk {
public:
    Chunk(std::vector<CubeData> cube_data, World& world,glm::vec2 chunk_coords);
    ~Chunk();

    std::vector<CubeData>& getCubeData();
    void RenderChunk();

private:
    VAO VAO1_;
    VBO VBO1_;
    EBO EBO1_;

    glm::vec2 chunk_coords_;

    unsigned int k_chunk_size_x_ = 16;
    unsigned int k_chunk_size_y_ = 512;
    unsigned int k_chunk_size_z_ = 16;

    int indices_size = 0;

    std::vector<CubeData> cube_data_;
    std::vector<unsigned int> indices;

    void GenerateFaces(World& world);
    void BindVAOAttributes(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);
    std::vector<unsigned int> AddIndices(unsigned int face_count);
    int CalculateIndex(int x, int y, int z) const;
};

struct CubeData {
    bool is_air;
    glm::vec3 position;
};
#endif // CHUNK_H
