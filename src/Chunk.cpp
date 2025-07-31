//
// Created by svjat on 7/27/2025.
//

#include "../header/Chunk.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <array>
#include <iostream>
#include <ostream>
#include <vector>

Chunk::Chunk(int tile_size) {
    this->tile_size_ = tile_size;
    this->chunk_xz_ = 16;
    this->chunk_y_ = 512;
    GenerateChunk();
    GenerateFaces();
}

Chunk::~Chunk() {
    VAO1_.Delete();
    VBO1_.Delete();
    EBO1_.Delete();
}

void Chunk::GenerateChunk() {
    std::array<float, 6> index_orderering = { 0, 1, 2, 2, 3, 0 };

    vertices_.reserve(chunk_xz_ * chunk_y_ * chunk_xz_);

    float offset = 0.5f;
    float vertex_index = 0;
    for (float x = 0; x < chunk_xz_; x++) {
        for (float y = 0; y < chunk_y_; y++) {
            for (float z = 0; z < chunk_xz_; z++) {
                // Top
                vertices_.emplace_back(Vertex(glm::vec3(x + offset, y + offset, z - offset), glm::vec2(0.f, 1.f)));
                vertices_.emplace_back(Vertex(glm::vec3(x + offset, y + offset, z + offset), glm::vec2(0.f, 0.f)));
                vertices_.emplace_back(Vertex(glm::vec3(x - offset, y + offset, z + offset), glm::vec2(1.f, 0.f)));
                vertices_.emplace_back(Vertex(glm::vec3(x - offset, y + offset, z - offset), glm::vec2(1.f, 1.f)));

                for (unsigned int i = 0; i < 6; i++) {
                    indices_.emplace_back(vertex_index + index_orderering[i]);
                }
                vertex_index += 4;

                // Bottom
                vertices_.emplace_back(Vertex(glm::vec3(x - offset, y - offset, z - offset), glm::vec2(1.f, 1.f)));
                vertices_.emplace_back(Vertex(glm::vec3(x - offset, y - offset, z + offset), glm::vec2(1.f, 0.f)));
                vertices_.emplace_back(Vertex(glm::vec3(x + offset, y - offset, z + offset), glm::vec2(0.f, 0.f)));
                vertices_.emplace_back(Vertex(glm::vec3(x + offset, y - offset, z - offset), glm::vec2(0.f, 1.f)));

                for (unsigned int i = 0; i < 6; i++) {
                    indices_.emplace_back(vertex_index + index_orderering[i]);
                }
                vertex_index += 4;

                // Back
                vertices_.emplace_back(Vertex(glm::vec3(x + offset, y - offset, z - offset), glm::vec2(1.f, 0.f)));
                vertices_.emplace_back(Vertex(glm::vec3(x + offset, y + offset, z - offset), glm::vec2(1.f, 1.f)));
                vertices_.emplace_back(Vertex(glm::vec3(x - offset, y + offset, z - offset), glm::vec2(0.f, 1.f)));
                vertices_.emplace_back(Vertex(glm::vec3(x - offset, y - offset, z - offset), glm::vec2(0.f, 0.f)));

                for (unsigned int i = 0; i < 6; i++) {
                    indices_.emplace_back(vertex_index + index_orderering[i]);
                }
                vertex_index += 4;

                // Front
                vertices_.emplace_back(Vertex(glm::vec3(x - offset, y - offset, z + offset), glm::vec2(1.f, 0.f)));
                vertices_.emplace_back(Vertex(glm::vec3(x - offset, y + offset, z + offset), glm::vec2(1.f, 1.f)));
                vertices_.emplace_back(Vertex(glm::vec3(x + offset, y + offset, z + offset), glm::vec2(0.f, 1.f)));
                vertices_.emplace_back(Vertex(glm::vec3(x + offset, y - offset, z + offset), glm::vec2(0.f, 0.f)));

                for (unsigned int i = 0; i < 6; i++) {
                    indices_.emplace_back(vertex_index + index_orderering[i]);
                }
                vertex_index += 4;

                // Left
                vertices_.emplace_back(Vertex(glm::vec3(x - offset, y + offset, z + offset), glm::vec2(0.f, 1.f)));
                vertices_.emplace_back(Vertex(glm::vec3(x - offset, y - offset, z + offset), glm::vec2(0.f, 0.f)));
                vertices_.emplace_back(Vertex(glm::vec3(x - offset, y - offset, z - offset), glm::vec2(1.f, 0.f)));
                vertices_.emplace_back(Vertex(glm::vec3(x - offset, y + offset, z - offset), glm::vec2(1.f, 1.f)));

                for (unsigned int i = 0; i < 6; i++) {
                    indices_.emplace_back(vertex_index + index_orderering[i]);
                }
                vertex_index += 4;

                // Right
                vertices_.emplace_back(Vertex(glm::vec3(x + offset, y + offset, z - offset), glm::vec2(0.f, 1.f)));
                vertices_.emplace_back(Vertex(glm::vec3(x + offset, y - offset, z - offset), glm::vec2(0.f, 0.f)));
                vertices_.emplace_back(Vertex(glm::vec3(x + offset, y - offset, z + offset), glm::vec2(1.f, 0.f)));
                vertices_.emplace_back(Vertex(glm::vec3(x + offset, y + offset, z + offset), glm::vec2(1.f, 1.f)));

                for (unsigned int i = 0; i < 6; i++) {
                    indices_.emplace_back(vertex_index + index_orderering[i]);
                }
                vertex_index += 4;
            }
        }
    }
}

void Chunk::GenerateFaces() {
    VAO1_.Bind();

    VBO1_.SetData(vertices_);
    EBO1_.SetData(indices_);

    VBO1_.Bind();
    EBO1_.Bind();

    VAO1_.LinkAttrib(0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
    VAO1_.LinkAttrib(1, 2, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
    // VAO1.LinkAttrib(VBO1,2,3,GL_FLOAT, 5 * sizeof(int), (void*)(5 * sizeof(float))); //Lighting normal isn't needed yet

    VAO1_.Unbind();
    VBO1_.Unbind();
    EBO1_.Unbind();
}

void Chunk::RenderChunk() {
    VAO1_.Bind();
    glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, 0);
}
