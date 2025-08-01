//
// Created by svjat on 7/27/2025.
//

#include "../header/Chunk.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <array>
#include <iostream>
#include <ostream>
#include <utility>
#include <vector>

Chunk::Chunk(std::vector<CubeData> cube_data) : cube_data_(cube_data) {
    GenerateFaces();
}

Chunk::~Chunk() {
    VAO1_.Delete();
    VBO1_.Delete();
    EBO1_.Delete();
    cube_data_.clear();
}

void Chunk::GenerateFaces() {
    std::vector<Vertex> vertices;

    unsigned int mesh_face_count = 0;
    // Cubes are drawn off their center positions
    // If I want to change offset, I would need to rewrite the code so the points are relative to some corner
    float cube_offset = .5f;

    for (int x = 0; x < k_chunk_size_x_; x++) {
        for (int y = 0; y < k_chunk_size_y_; y++) {
            for (int z = 0; z < k_chunk_size_z_; z++) {

                int current_cube = CalculateIndex(x, y, z);
                if (cube_data_[current_cube].is_air) {
                    continue;
                }

                // Top
                int cube_above = CalculateIndex(x, y + 1, z);
                if (y + 1 >= k_chunk_size_y_ || cube_data_[cube_above].is_air) {
                    vertices.emplace_back(Vertex(glm::vec3(x + cube_offset, y + cube_offset, z - cube_offset), glm::vec2(0.f, 1.f)));
                    vertices.emplace_back(Vertex(glm::vec3(x + cube_offset, y + cube_offset, z + cube_offset), glm::vec2(0.f, 0.f)));
                    vertices.emplace_back(Vertex(glm::vec3(x - cube_offset, y + cube_offset, z + cube_offset), glm::vec2(1.f, 0.f)));
                    vertices.emplace_back(Vertex(glm::vec3(x - cube_offset, y + cube_offset, z - cube_offset), glm::vec2(1.f, 1.f)));

                    mesh_face_count++;
                }

                // Bottom
                int cube_below = CalculateIndex(x, y - 1, z);
                if (y <= 0 || cube_data_[cube_below].is_air) {
                    vertices.emplace_back(Vertex(glm::vec3(x - cube_offset, y - cube_offset, z - cube_offset), glm::vec2(1.f, 1.f)));
                    vertices.emplace_back(Vertex(glm::vec3(x - cube_offset, y - cube_offset, z + cube_offset), glm::vec2(1.f, 0.f)));
                    vertices.emplace_back(Vertex(glm::vec3(x + cube_offset, y - cube_offset, z + cube_offset), glm::vec2(0.f, 0.f)));
                    vertices.emplace_back(Vertex(glm::vec3(x + cube_offset, y - cube_offset, z - cube_offset), glm::vec2(0.f, 1.f)));

                    mesh_face_count++;
                }

                // Back
                int cube_back = CalculateIndex(x, y, z - 1);
                if (z <= 0 || cube_data_[cube_back].is_air) {
                    vertices.emplace_back(Vertex(glm::vec3(x + cube_offset, y - cube_offset, z - cube_offset), glm::vec2(1.f, 0.f)));
                    vertices.emplace_back(Vertex(glm::vec3(x + cube_offset, y + cube_offset, z - cube_offset), glm::vec2(1.f, 1.f)));
                    vertices.emplace_back(Vertex(glm::vec3(x - cube_offset, y + cube_offset, z - cube_offset), glm::vec2(0.f, 1.f)));
                    vertices.emplace_back(Vertex(glm::vec3(x - cube_offset, y - cube_offset, z - cube_offset), glm::vec2(0.f, 0.f)));

                    mesh_face_count++;
                }

                // Front
                int cube_front = CalculateIndex(x, y, z + 1);
                if (z + 1 >= k_chunk_size_z_ || cube_data_[cube_front].is_air) {
                    vertices.emplace_back(Vertex(glm::vec3(x - cube_offset, y - cube_offset, z + cube_offset), glm::vec2(1.f, 0.f)));
                    vertices.emplace_back(Vertex(glm::vec3(x - cube_offset, y + cube_offset, z + cube_offset), glm::vec2(1.f, 1.f)));
                    vertices.emplace_back(Vertex(glm::vec3(x + cube_offset, y + cube_offset, z + cube_offset), glm::vec2(0.f, 1.f)));
                    vertices.emplace_back(Vertex(glm::vec3(x + cube_offset, y - cube_offset, z + cube_offset), glm::vec2(0.f, 0.f)));

                    mesh_face_count++;
                }

                // Left
                int cube_left = CalculateIndex(x - 1, y, z);
                if (x <= 0 || cube_data_[cube_left].is_air) {
                    vertices.emplace_back(Vertex(glm::vec3(x - cube_offset, y + cube_offset, z + cube_offset), glm::vec2(0.f, 1.f)));
                    vertices.emplace_back(Vertex(glm::vec3(x - cube_offset, y - cube_offset, z + cube_offset), glm::vec2(0.f, 0.f)));
                    vertices.emplace_back(Vertex(glm::vec3(x - cube_offset, y - cube_offset, z - cube_offset), glm::vec2(1.f, 0.f)));
                    vertices.emplace_back(Vertex(glm::vec3(x - cube_offset, y + cube_offset, z - cube_offset), glm::vec2(1.f, 1.f)));

                    mesh_face_count++;
                }

                // Right
                int cube_right = CalculateIndex(x + 1, y, z);
                if (x + 1 >= k_chunk_size_x_ || cube_data_[cube_right].is_air) {
                    vertices.emplace_back(Vertex(glm::vec3(x + cube_offset, y + cube_offset, z - cube_offset), glm::vec2(0.f, 1.f)));
                    vertices.emplace_back(Vertex(glm::vec3(x + cube_offset, y - cube_offset, z - cube_offset), glm::vec2(0.f, 0.f)));
                    vertices.emplace_back(Vertex(glm::vec3(x + cube_offset, y - cube_offset, z + cube_offset), glm::vec2(1.f, 0.f)));
                    vertices.emplace_back(Vertex(glm::vec3(x + cube_offset, y + cube_offset, z + cube_offset), glm::vec2(1.f, 1.f)));

                    mesh_face_count++;
                }
            }
        }
    }
    std::vector<unsigned int> indices = AddIndices(mesh_face_count);
    BindVAOAttributes(vertices, indices);
}

void Chunk::BindVAOAttributes(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices) {
    VAO1_.Bind();

    VBO1_.SetData(vertices);
    EBO1_.SetData(indices);

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
    glDrawElements(GL_TRIANGLES, indices_size, GL_UNSIGNED_INT, 0);
}

int Chunk::CalculateIndex(int x, int y, int z) const {
    // Every iteration of y is equal to 1 iteration of ChunkZSize_, same for x and ChunkYSize_
    return (x * k_chunk_size_y_) + (y * k_chunk_size_z_ ) + (z);
}

std::vector<unsigned int> Chunk::AddIndices(unsigned int face_count) {
    std::vector<unsigned int> indices;
    std::array<float, 6> index_ordering = { 0, 1, 2, 2, 3, 0 };
    float vertex_index = 0;

    for (int x = 0; x < face_count; x++) {
        for (unsigned int i = 0; i < 6; i++) {
            indices.emplace_back(vertex_index + index_ordering[i]);
        }
        vertex_index += 4;
    }

    indices_size = indices.size();

    return indices;
}

std::vector<CubeData>* Chunk::getCubeData() {
    return &cube_data_;
}
