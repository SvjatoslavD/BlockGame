//
// Created by svjat on 7/27/2025.
//

#include "../header/Chunk.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <array>
#include <iostream>
#include <ostream>
#include <vector>

Chunk::Chunk(int tileSize) {
    this->tileSize = tileSize;
    this->chunkXZ = 16;
    this->chunkY = 512;
    GenerateChunk();
    GenerateFaces();
}

Chunk::~Chunk() {
    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();
}

void Chunk::GenerateChunk() {
    std::array<float, 6> indexOrderering = { 0, 1, 2, 2, 3, 0 };

    vertices.reserve(chunkXZ * chunkY * chunkXZ);

    float offset = 0.5f;
    float vertexIndex = 0;
    for (float x = 0; x < chunkXZ; x++) {
        for (float y = 0; y < chunkY; y++) {
            for (float z = 0; z < chunkXZ; z++) {
                // Top
                vertices.emplace_back(Vertex(glm::vec3(x + offset, y + offset, z - offset), glm::vec2(0.f, 1.f)));
                vertices.emplace_back(Vertex(glm::vec3(x + offset, y + offset, z + offset), glm::vec2(0.f, 0.f)));
                vertices.emplace_back(Vertex(glm::vec3(x - offset, y + offset, z + offset), glm::vec2(1.f, 0.f)));
                vertices.emplace_back(Vertex(glm::vec3(x - offset, y + offset, z - offset), glm::vec2(1.f, 1.f)));

                for (unsigned int i = 0; i < 6; i++) {
                    indices.emplace_back(vertexIndex + indexOrderering[i]);
                }
                vertexIndex += 4;

                // Bottom
                vertices.emplace_back(Vertex(glm::vec3(x - offset, y - offset, z - offset), glm::vec2(1.f, 1.f)));
                vertices.emplace_back(Vertex(glm::vec3(x - offset, y - offset, z + offset), glm::vec2(1.f, 0.f)));
                vertices.emplace_back(Vertex(glm::vec3(x + offset, y - offset, z + offset), glm::vec2(0.f, 0.f)));
                vertices.emplace_back(Vertex(glm::vec3(x + offset, y - offset, z - offset), glm::vec2(0.f, 1.f)));

                for (unsigned int i = 0; i < 6; i++) {
                    indices.emplace_back(vertexIndex + indexOrderering[i]);
                }
                vertexIndex += 4;

                // Back
                vertices.emplace_back(Vertex(glm::vec3(x + offset, y - offset, z - offset), glm::vec2(1.f, 0.f)));
                vertices.emplace_back(Vertex(glm::vec3(x + offset, y + offset, z - offset), glm::vec2(1.f, 1.f)));
                vertices.emplace_back(Vertex(glm::vec3(x - offset, y + offset, z - offset), glm::vec2(0.f, 1.f)));
                vertices.emplace_back(Vertex(glm::vec3(x - offset, y - offset, z - offset), glm::vec2(0.f, 0.f)));

                for (unsigned int i = 0; i < 6; i++) {
                    indices.emplace_back(vertexIndex + indexOrderering[i]);
                }
                vertexIndex += 4;

                // Front
                vertices.emplace_back(Vertex(glm::vec3(x - offset, y - offset, z + offset), glm::vec2(1.f, 0.f)));
                vertices.emplace_back(Vertex(glm::vec3(x - offset, y + offset, z + offset), glm::vec2(1.f, 1.f)));
                vertices.emplace_back(Vertex(glm::vec3(x + offset, y + offset, z + offset), glm::vec2(0.f, 1.f)));
                vertices.emplace_back(Vertex(glm::vec3(x + offset, y - offset, z + offset), glm::vec2(0.f, 0.f)));

                for (unsigned int i = 0; i < 6; i++) {
                    indices.emplace_back(vertexIndex + indexOrderering[i]);
                }
                vertexIndex += 4;

                // Left
                vertices.emplace_back(Vertex(glm::vec3(x - offset, y + offset, z + offset), glm::vec2(0.f, 1.f)));
                vertices.emplace_back(Vertex(glm::vec3(x - offset, y - offset, z + offset), glm::vec2(0.f, 0.f)));
                vertices.emplace_back(Vertex(glm::vec3(x - offset, y - offset, z - offset), glm::vec2(1.f, 0.f)));
                vertices.emplace_back(Vertex(glm::vec3(x - offset, y + offset, z - offset), glm::vec2(1.f, 1.f)));

                for (unsigned int i = 0; i < 6; i++) {
                    indices.emplace_back(vertexIndex + indexOrderering[i]);
                }
                vertexIndex += 4;

                // Right
                vertices.emplace_back(Vertex(glm::vec3(x + offset, y + offset, z - offset), glm::vec2(0.f, 1.f)));
                vertices.emplace_back(Vertex(glm::vec3(x + offset, y - offset, z - offset), glm::vec2(0.f, 0.f)));
                vertices.emplace_back(Vertex(glm::vec3(x + offset, y - offset, z + offset), glm::vec2(1.f, 0.f)));
                vertices.emplace_back(Vertex(glm::vec3(x + offset, y + offset, z + offset), glm::vec2(1.f, 1.f)));

                for (unsigned int i = 0; i < 6; i++) {
                    indices.emplace_back(vertexIndex + indexOrderering[i]);
                }
                vertexIndex += 4;
            }
        }
    }
}

void Chunk::GenerateFaces() {
    VAO1.Bind();

    VBO1.SetData(vertices);
    EBO1.SetData(indices);

    VBO1.Bind();
    EBO1.Bind();

    VAO1.LinkAttrib(0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
    VAO1.LinkAttrib(1, 2, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
    // VAO1.LinkAttrib(VBO1,2,3,GL_FLOAT, 5 * sizeof(int), (void*)(5 * sizeof(float))); //Lighting normal isn't needed yet

    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();
}

void Chunk::RenderChunk() {
    VAO1.Bind();
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}
