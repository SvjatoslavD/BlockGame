//
// Created by svjat on 7/27/2025.
//

#ifndef CHUNK_H
#define CHUNK_H

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <vector>
#include "EBO.h"
#include "VAO.h"
#include "VBO.h"

struct CubeData;

class Chunk {
public:
    explicit Chunk(int tileSize);
    ~Chunk();

    void RenderChunk();

private:
    int tile_size_;
    VAO VAO1_;
    VBO VBO1_;
    EBO EBO1_;

    unsigned int chunk_xz_size_;
    unsigned int chunk_y_size_;

    std::vector<CubeData> cube_locations_;
    std::vector<Vertex> vertices_;
    std::vector<unsigned int> indices_;

    void GenerateChunkData();
    void GenerateChunk();
    void GenerateFaces();
    float CalculateIndex(float x, float y, float z) const;
};

struct CubeData {
    bool is_air;
    glm::vec3 position;
};
#endif // CHUNK_H
