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
    unsigned int chunk_xz_;
    unsigned int chunk_y_;
    unsigned int triangle_count_;
    std::vector<Vertex> vertices_;
    std::vector<unsigned int> indices_;

    void GenerateChunk();
    void GenerateFaces();
};
#endif // CHUNK_H
