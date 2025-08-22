//
// Created by svjat on 7/27/2025.
//

#include "Chunk.h"
#include "World.h"

#include <array>
#include <vector>

Chunk::Chunk(std::vector<CubeData> cube_data, World& world, glm::ivec2 chunk_coords) : cube_data_(cube_data), chunk_coords_(chunk_coords) {
    GenerateFaces(world);
}

Chunk::~Chunk() {
    VAO1_.Delete();
    VBO1_.Delete();
    EBO1_.Delete();
    cube_data_.clear();
}

void Chunk::GenerateFaces(World& world) {
    int chunk_x = chunk_coords_.x;
    int chunk_z = chunk_coords_.y;

    std::vector<CubeData> face;

    unsigned int mesh_face_count = 0;
    // Cubes are drawn off their back, bottom left corner
    int cube_offset = 1;

    glm::ivec2 key_back = glm::ivec2(chunk_x, chunk_z - 1);
    std::vector<CubeData>* chunk_back;
    if (world.ChunkExists(key_back)) { chunk_back = &world.getChunkData(key_back); }
    else { chunk_back = nullptr;}

    glm::ivec2 key_front = glm::ivec2(chunk_x,chunk_z + 1);
    std::vector<CubeData>* chunk_front;
    if (world.ChunkExists(key_front)) { chunk_front = &world.getChunkData(key_front); }
    else { chunk_front = nullptr;}

    glm::ivec2 key_left = glm::ivec2(chunk_x - 1, chunk_z);
    std::vector<CubeData>* chunk_left;
    if (world.ChunkExists(key_left)) { chunk_left = &world.getChunkData(key_left); }
    else { chunk_left = nullptr;}

    glm::ivec2 key_right = glm::ivec2(chunk_x + 1,chunk_z);
    std::vector<CubeData>* chunk_right;
    if (world.ChunkExists(key_right)) { chunk_right = &world.getChunkData(key_right); }
    else { chunk_right = nullptr;}

    for (int x = 0; x < k_chunk_size_x_; x++) {
        for (int z = 0; z < k_chunk_size_z_; z++) {
            for (int y = 0; y < k_chunk_size_y_; y++) {
                int current_cube = CalculateIndex(x, y, z);
                if (cube_data_[current_cube].is_air) {
                    continue;
                }

                Block block = SolidBlocks[cube_data_[current_cube].type];

                // Top
                int cube_above = CalculateIndex(x, y + cube_offset, z);
                if (y + 1 >= k_chunk_size_y_ || cube_data_[cube_above].is_air) {
                    vertices.emplace_back(Vertex(glm::vec3(x + cube_offset, y + cube_offset, z), glm::vec3(0, 1, block.faces[0])));
                    vertices.emplace_back(Vertex(glm::vec3(x + cube_offset, y + cube_offset, z + cube_offset), glm::vec3(0, 0, block.faces[0])));
                    vertices.emplace_back(Vertex(glm::vec3(x, y + cube_offset, z + cube_offset), glm::vec3(1, 0, block.faces[0])));
                    vertices.emplace_back(Vertex(glm::vec3(x, y + cube_offset, z), glm::vec3(1, 1, block.faces[0])));

                    mesh_face_count++;
                }

                // Bottom
                int cube_below = CalculateIndex(x, y - cube_offset, z);
                // Make sure we don't render faces below the chunk for now
                if (y != 0 && cube_data_[cube_below].is_air) {
                    vertices.emplace_back(Vertex(glm::vec3(x, y, z), glm::vec3(1, 1, block.faces[1])));
                    vertices.emplace_back(Vertex(glm::vec3(x, y, z + cube_offset), glm::vec3(1, 0, block.faces[1])));
                    vertices.emplace_back(Vertex(glm::vec3(x + cube_offset, y, z + cube_offset), glm::vec3(0, 0, block.faces[1])));
                    vertices.emplace_back(Vertex(glm::vec3(x + cube_offset, y, z), glm::vec3(0, 1, block.faces[1])));

                    mesh_face_count++;
                }

                // need to check neighboring chunks

                // Back
                bool draw_back_face = false;

                if (z == 0) {
                    int index = CalculateIndex(x, y, k_chunk_size_z_- 1);
                    if (chunk_back != nullptr) {
                        if (chunk_back->operator[](index).is_air) {
                            draw_back_face = true;
                        }
                    }
                }
                else {
                    int cube_back = CalculateIndex(x, y, z - 1);
                    if (cube_data_[cube_back].is_air) {
                        draw_back_face = true;
                    }
                }

                if (draw_back_face) {
                    vertices.emplace_back(Vertex(glm::vec3(x + cube_offset, y , z ), glm::vec3(1, 0, block.faces[2])));
                    vertices.emplace_back(Vertex(glm::vec3(x + cube_offset, y + cube_offset, z ), glm::vec3(1, 1, block.faces[2])));
                    vertices.emplace_back(Vertex(glm::vec3(x , y + cube_offset, z ), glm::vec3(0, 1, block.faces[2])));
                    vertices.emplace_back(Vertex(glm::vec3(x , y , z ), glm::vec3(0, 0, block.faces[2])));

                    mesh_face_count++;
                }

                // Front
                bool draw_front_face = false;

                if (z == k_chunk_size_z_ - 1) {
                    if (chunk_front != nullptr) {
                        int index = CalculateIndex(x, y, 0);
                        if (chunk_front->operator[](index).is_air) {
                            draw_front_face = true;
                        }
                    }
                }
                else {
                    int cube_front = CalculateIndex(x, y, z + 1);
                    if (cube_data_[cube_front].is_air) {
                        draw_front_face = true;
                    }
                }

                if (draw_front_face) {
                    vertices.emplace_back(Vertex(glm::vec3(x , y , z + cube_offset), glm::vec3(1, 0, block.faces[3])));
                    vertices.emplace_back(Vertex(glm::vec3(x , y + cube_offset, z + cube_offset), glm::vec3(1, 1, block.faces[3])));
                    vertices.emplace_back(Vertex(glm::vec3(x + cube_offset, y + cube_offset, z + cube_offset), glm::vec3(0, 1, block.faces[3])));
                    vertices.emplace_back(Vertex(glm::vec3(x + cube_offset, y , z + cube_offset), glm::vec3(0, 0, block.faces[3])));

                    mesh_face_count++;
                }

                // Left
                bool draw_left_face = false;

                if (x == 0) {
                    if (chunk_left != nullptr) {
                        int index = CalculateIndex(k_chunk_size_x_ - 1, y, z);
                        if (chunk_left->operator[](index).is_air) {
                            draw_left_face = true;
                        }
                    }
                }
                else {
                    int cube_left = CalculateIndex(x - 1, y, z);
                    if (cube_data_[cube_left].is_air) {
                        draw_left_face = true;
                    }
                }

                if (draw_left_face) {
                    vertices.emplace_back(Vertex(glm::vec3(x , y + cube_offset, z + cube_offset), glm::vec3(0, 1, block.faces[4])));
                    vertices.emplace_back(Vertex(glm::vec3(x , y , z + cube_offset), glm::vec3(0, 0, block.faces[4])));
                    vertices.emplace_back(Vertex(glm::vec3(x , y , z ), glm::vec3(1, 0, block.faces[4])));
                    vertices.emplace_back(Vertex(glm::vec3(x , y + cube_offset, z ), glm::vec3(1, 1, block.faces[4])));

                    mesh_face_count++;
                }

                // Right
                bool draw_right_face = false;

                if (x == k_chunk_size_x_ - 1) {
                    if (chunk_right != nullptr) {
                        int index = CalculateIndex(0, y, z);
                        if (chunk_right->operator[](index).is_air) {
                            draw_right_face = true;
                        }
                    }
                }
                else {
                    int cube_right = CalculateIndex(x + 1, y, z);
                    if (cube_data_[cube_right].is_air) {
                        draw_right_face = true;
                    }
                }

                if (draw_right_face) {
                    vertices.emplace_back(Vertex(glm::vec3(x + cube_offset, y + cube_offset, z ), glm::vec3(0, 1, block.faces[5])));
                    vertices.emplace_back(Vertex(glm::vec3(x + cube_offset, y , z ), glm::vec3(0, 0, block.faces[5])));
                    vertices.emplace_back(Vertex(glm::vec3(x + cube_offset, y , z + cube_offset), glm::vec3(1, 0, block.faces[5])));
                    vertices.emplace_back(Vertex(glm::vec3(x + cube_offset, y + cube_offset, z + cube_offset), glm::vec3(1, 1, block.faces[5])));

                    mesh_face_count++;
                }
            }
        }
    }
    indices = AddIndices(mesh_face_count);
}

void Chunk::BindVAOAttributes() {
    VAO1_.Bind();

    VBO1_.SetData(vertices);
    EBO1_.SetData(indices);

    VBO1_.Bind();
    EBO1_.Bind();

    VAO1_.LinkAttrib(0,3,GL_UNSIGNED_SHORT, sizeof(Vertex), (void*)0);
    VAO1_.LinkAttrib(1,3,GL_UNSIGNED_BYTE, sizeof(Vertex), (void*)offsetof(Vertex, tex_coords));
    // VAO1.LinkAttrib(VBO1,2,3,GL_FLOAT, 5 * sizeof(int), (void*)(5 * sizeof(float))); //Lighting normal isn't needed yet

    VAO1_.Unbind();
    VBO1_.Unbind();
    EBO1_.Unbind();
}

void Chunk::RenderChunk() {
    if (chunk_changed_) {
        BindVAOAttributes();
        chunk_changed_ = false;
    }
    VAO1_.Bind();
    glDrawElements(GL_TRIANGLES, indices_size, GL_UNSIGNED_INT, 0);
}

int Chunk::CalculateIndex(const int x, const int y, const int z) const {
    // Every iteration of z is equal to 1 iteration of y_size_, and every iteration of x is equal to an iteration of z_size (which includes y_size)
    return (x * k_chunk_size_z_ * k_chunk_size_y_) + (z * k_chunk_size_y_ ) + (y);
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

std::vector<CubeData>& Chunk::getCubeData() {
    return cube_data_;
}
