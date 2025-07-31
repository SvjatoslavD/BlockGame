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
    this->chunk_xz_size_ = 16;
    this->chunk_y_size_ = 64;
    GenerateChunkData();
    GenerateChunk();
    GenerateFaces();
}

Chunk::~Chunk() {
    VAO1_.Delete();
    VBO1_.Delete();
    EBO1_.Delete();
}

void Chunk::GenerateChunkData() {
    // This var is temporary to test chunk generation
    int fill_style = 1;
    cube_locations_.reserve(chunk_xz_size_*chunk_y_size_*chunk_xz_size_);
    for (int x = 0; x < chunk_xz_size_; x++) {
        for (int y = 0; y < chunk_y_size_; y++) {
            for (int z = 0; z < chunk_xz_size_; z++) {
                CubeData cube_data{};
                if (fill_style == 0) {
                    cube_data.is_air = true;
                }
                else if (fill_style == 1) {
                    cube_data.is_air = false;
                }
                else if (fill_style == 2) {
                    if ((x + y + z) % 2 == 0) {
                        cube_data.is_air = true;
                    }
                    else {
                        cube_data.is_air = false;
                    }
                }
                cube_data.position = glm::vec3(x, y, z);
                cube_locations_.emplace_back(cube_data);
            }
        }
    }
}

void Chunk::GenerateChunk() {
    std::array<float, 6> index_orderering = { 0, 1, 2, 2, 3, 0 };

    // Cubes are drawn off their center positions
    // If I want to change offset, I would need to rewrite the code so the points are relative to some corner
    float offset = .5f;
    float vertex_index = 0;
    for (int x = 0; x < chunk_xz_size_; x++) {
        for (int y = 0; y < chunk_y_size_; y++) {
            for (int z = 0; z < chunk_xz_size_; z++) {
                float current_cube = CalculateIndex(x, y, z);
                if (cube_locations_[current_cube].is_air) {
                    continue;
                }

                // Top
                float face_above = CalculateIndex(x, y + 1, z);
                if (y + 1 >= chunk_y_size_ || cube_locations_[face_above].is_air) {
                    vertices_.emplace_back(Vertex(glm::vec3(x + offset, y + offset, z - offset), glm::vec2(0.f, 1.f)));
                    vertices_.emplace_back(Vertex(glm::vec3(x + offset, y + offset, z + offset), glm::vec2(0.f, 0.f)));
                    vertices_.emplace_back(Vertex(glm::vec3(x - offset, y + offset, z + offset), glm::vec2(1.f, 0.f)));
                    vertices_.emplace_back(Vertex(glm::vec3(x - offset, y + offset, z - offset), glm::vec2(1.f, 1.f)));

                    for (unsigned int i = 0; i < 6; i++) {
                        indices_.emplace_back(vertex_index + index_orderering[i]);
                    }
                    vertex_index += 4;
                }

                // Bottom
                float face_below = CalculateIndex(x, y - 1, z);
                if (y <= 0 || cube_locations_[face_below].is_air) {
                    vertices_.emplace_back(Vertex(glm::vec3(x - offset, y - offset, z - offset), glm::vec2(1.f, 1.f)));
                    vertices_.emplace_back(Vertex(glm::vec3(x - offset, y - offset, z + offset), glm::vec2(1.f, 0.f)));
                    vertices_.emplace_back(Vertex(glm::vec3(x + offset, y - offset, z + offset), glm::vec2(0.f, 0.f)));
                    vertices_.emplace_back(Vertex(glm::vec3(x + offset, y - offset, z - offset), glm::vec2(0.f, 1.f)));

                    for (unsigned int i = 0; i < 6; i++) {
                        indices_.emplace_back(vertex_index + index_orderering[i]);
                    }
                    vertex_index += 4;
                }

                // Back
                float face_back = CalculateIndex(x, y, z - 1);
                if (z <= 0 || cube_locations_[face_back].is_air) {
                    vertices_.emplace_back(Vertex(glm::vec3(x + offset, y - offset, z - offset), glm::vec2(1.f, 0.f)));
                    vertices_.emplace_back(Vertex(glm::vec3(x + offset, y + offset, z - offset), glm::vec2(1.f, 1.f)));
                    vertices_.emplace_back(Vertex(glm::vec3(x - offset, y + offset, z - offset), glm::vec2(0.f, 1.f)));
                    vertices_.emplace_back(Vertex(glm::vec3(x - offset, y - offset, z - offset), glm::vec2(0.f, 0.f)));

                    for (unsigned int i = 0; i < 6; i++) {
                        indices_.emplace_back(vertex_index + index_orderering[i]);
                    }
                    vertex_index += 4;
                }

                // Front
                float face_front = CalculateIndex(x, y, z + 1);
                if (z + 1 >= chunk_xz_size_ || cube_locations_[face_front].is_air) {
                    vertices_.emplace_back(Vertex(glm::vec3(x - offset, y - offset, z + offset), glm::vec2(1.f, 0.f)));
                    vertices_.emplace_back(Vertex(glm::vec3(x - offset, y + offset, z + offset), glm::vec2(1.f, 1.f)));
                    vertices_.emplace_back(Vertex(glm::vec3(x + offset, y + offset, z + offset), glm::vec2(0.f, 1.f)));
                    vertices_.emplace_back(Vertex(glm::vec3(x + offset, y - offset, z + offset), glm::vec2(0.f, 0.f)));

                    for (unsigned int i = 0; i < 6; i++) {
                        indices_.emplace_back(vertex_index + index_orderering[i]);
                    }
                    vertex_index += 4;
                }

                // Left
                float face_left = CalculateIndex(x - 1, y, z);
                if (x <= 0 || cube_locations_[face_left].is_air) {
                    vertices_.emplace_back(Vertex(glm::vec3(x - offset, y + offset, z + offset), glm::vec2(0.f, 1.f)));
                    vertices_.emplace_back(Vertex(glm::vec3(x - offset, y - offset, z + offset), glm::vec2(0.f, 0.f)));
                    vertices_.emplace_back(Vertex(glm::vec3(x - offset, y - offset, z - offset), glm::vec2(1.f, 0.f)));
                    vertices_.emplace_back(Vertex(glm::vec3(x - offset, y + offset, z - offset), glm::vec2(1.f, 1.f)));

                    for (unsigned int i = 0; i < 6; i++) {
                        indices_.emplace_back(vertex_index + index_orderering[i]);
                    }
                    vertex_index += 4;
                }

                // Right
                float face_right = CalculateIndex(x + 1, y, z);
                if (x + 1 >= chunk_xz_size_ || cube_locations_[face_right].is_air) {
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

float Chunk::CalculateIndex(float x, float y, float z) const {
    return (x * chunk_y_size_ * chunk_xz_size_) + (y * chunk_xz_size_ ) + (z );
}
