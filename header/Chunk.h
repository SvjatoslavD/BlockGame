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
    Chunk(int tileSize);
    ~Chunk();

    void RenderChunk();

private:
    int tileSize;
    VAO VAO1;
    VBO VBO1;
    EBO EBO1;
    unsigned int chunkXZ;
    unsigned int chunkY;
    unsigned int triangleCount;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    void GenerateChunk();
    void GenerateFaces();
};
#endif // CHUNK_H
