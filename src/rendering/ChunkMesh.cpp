//
// Created by svjat on 8/29/2025.
//

#include "ChunkMesh.h"
#include "World.h"

void ChunkMesh::GenerateMeshData(glm::ivec3 chunk_pos, World* world, std::vector<CubeData>& cubes) {
    // Cubes are drawn off their back, bottom left corner
    int cube_offset = 1;

	glm::ivec3 key_top = glm::ivec3(chunk_pos.x, chunk_pos.y + 1, chunk_pos.z);
	std::vector<CubeData>* chunk_top;
	if (world->ChunkExists(key_top)) { chunk_top = &world->getChunkData(key_top); }
	else { chunk_top = nullptr;}

	glm::ivec3 key_bottom = glm::ivec3(chunk_pos.x, chunk_pos.y - 1, chunk_pos.z);
	std::vector<CubeData>* chunk_bottom;
	if (world->ChunkExists(key_bottom)) { chunk_bottom = &world->getChunkData(key_bottom); }
	else { chunk_bottom = nullptr;}

    glm::ivec3 key_back = glm::ivec3(chunk_pos.x, chunk_pos.y, chunk_pos.z - 1);
    std::vector<CubeData>* chunk_back;
    if (world->ChunkExists(key_back)) { chunk_back = &world->getChunkData(key_back); }
    else { chunk_back = nullptr;}

    glm::ivec3 key_front = glm::ivec3(chunk_pos.x,chunk_pos.y,chunk_pos.z + 1);
    std::vector<CubeData>* chunk_front;
    if (world->ChunkExists(key_front)) { chunk_front = &world->getChunkData(key_front); }
    else { chunk_front = nullptr;}

    glm::ivec3 key_left = glm::ivec3(chunk_pos.x - 1, chunk_pos.y,chunk_pos.z);
    std::vector<CubeData>* chunk_left;
    if (world->ChunkExists(key_left)) { chunk_left = &world->getChunkData(key_left); }
    else { chunk_left = nullptr;}

    glm::ivec3 key_right = glm::ivec3(chunk_pos.x + 1,chunk_pos.y,chunk_pos.z);
    std::vector<CubeData>* chunk_right;
    if (world->ChunkExists(key_right)) { chunk_right = &world->getChunkData(key_right); }
    else { chunk_right = nullptr;}

    for (int x = 0; x < k_chunk_size_; x++) {
		for (int z = 0; z < k_chunk_size_; z++) {
			for (int y = 0; y < k_chunk_size_; y++) {

                int current_cube = CalculateIndex(x, y, z);
                if (cubes[current_cube].is_air) {
                    continue;
                }

                Block block = SolidBlocks[cubes[current_cube].type];

                // Top
            	bool draw_top_face = false;

            	if (y == k_chunk_size_ - 1) {
            		int index = CalculateIndex(x, 0, z);
            		if (chunk_top != nullptr) {
            			if (chunk_top->operator[](index).is_air) {
            				draw_top_face = true;
            			}
            		}
            		else {
            			draw_top_face = true;
            		}
            	}
            	else {
            		int cube_top = CalculateIndex(x, y + 1, z);
            		if (cubes[cube_top].is_air) {
            			draw_top_face = true;
            		}
            	}
                if (draw_top_face) {
                	auto normal = glm::u8vec3(0, 1, 0);
                    mesh_data_.vertices.emplace_back(Vertex(glm::vec3(x + cube_offset, y + cube_offset, z), glm::vec3(0, 1, block.faces[0]),normal));
                    mesh_data_.vertices.emplace_back(Vertex(glm::vec3(x + cube_offset, y + cube_offset, z + cube_offset), glm::vec3(0, 0, block.faces[0]),normal));
                    mesh_data_.vertices.emplace_back(Vertex(glm::vec3(x, y + cube_offset, z + cube_offset), glm::vec3(1, 0, block.faces[0]),normal));
                    mesh_data_.vertices.emplace_back(Vertex(glm::vec3(x, y + cube_offset, z), glm::vec3(1, 1, block.faces[0]),normal));

                    mesh_data_.face_count++;
                }

                // Bottom
            	bool draw_bottom_face = false;

            	if (y == 0) {
            		int index = CalculateIndex(x, k_chunk_size_ - 1, z);
            		if (chunk_bottom != nullptr) {
            			if (chunk_bottom->operator[](index).is_air) {
            				draw_bottom_face = true;
            			}
            		}
            		else {
            			draw_bottom_face = true;
            		}
            	}
            	else {
            		int cube_bottom = CalculateIndex(x, y - 1, z);
            		if (cubes[cube_bottom].is_air) {
            			draw_bottom_face = true;
            		}
            	}
                if (draw_bottom_face) {
                	auto normal = glm::u8vec3(0, -1, 0);
                    mesh_data_.vertices.emplace_back(Vertex(glm::vec3(x, y, z), glm::vec3(1, 1, block.faces[1]),normal));
                    mesh_data_.vertices.emplace_back(Vertex(glm::vec3(x, y, z + cube_offset), glm::vec3(1, 0, block.faces[1]),normal));
                    mesh_data_.vertices.emplace_back(Vertex(glm::vec3(x + cube_offset, y, z + cube_offset), glm::vec3(0, 0, block.faces[1]),normal));
                    mesh_data_.vertices.emplace_back(Vertex(glm::vec3(x + cube_offset, y, z), glm::vec3(0, 1, block.faces[1]),normal));

                    mesh_data_.face_count++;
                }

                // need to check neighboring chunks

                // Back
                bool draw_back_face = false;

                if (z == 0) {
                    int index = CalculateIndex(x, y, k_chunk_size_- 1);
                    if (chunk_back != nullptr) {
                        if (chunk_back->operator[](index).is_air) {
                            draw_back_face = true;
                        }
                    }
                	else {
                		draw_back_face = true;
                	}
                }
                else {
                    int cube_back = CalculateIndex(x, y, z - 1);
                    if (cubes[cube_back].is_air) {
                        draw_back_face = true;
                    }
                }

                if (draw_back_face) {
                	auto normal = glm::u8vec3(0, 0, -1);
                    mesh_data_.vertices.emplace_back(Vertex(glm::vec3(x + cube_offset, y , z ), glm::vec3(1, 0, block.faces[2]),normal));
                    mesh_data_.vertices.emplace_back(Vertex(glm::vec3(x + cube_offset, y + cube_offset, z ), glm::vec3(1, 1, block.faces[2]),normal));
                    mesh_data_.vertices.emplace_back(Vertex(glm::vec3(x , y + cube_offset, z ), glm::vec3(0, 1, block.faces[2]), normal));
                    mesh_data_.vertices.emplace_back(Vertex(glm::vec3(x , y , z ), glm::vec3(0, 0, block.faces[2]), normal));

                    mesh_data_.face_count++;
                }

                // Front
                bool draw_front_face = false;

                if (z == k_chunk_size_ - 1) {
                    if (chunk_front != nullptr) {
                        int index = CalculateIndex(x, y, 0);
                        if (chunk_front->operator[](index).is_air) {
                            draw_front_face = true;
                        }
                    }
                	else {
                		draw_front_face = true;
                	}
                }
                else {
                    int cube_front = CalculateIndex(x, y, z + 1);
                    if (cubes[cube_front].is_air) {
                        draw_front_face = true;
                    }
                }

                if (draw_front_face) {
                	auto normal = glm::u8vec3(0, 0, 1);
                    mesh_data_.vertices.emplace_back(Vertex(glm::vec3(x , y , z + cube_offset), glm::vec3(1, 0, block.faces[3]),normal));
                    mesh_data_.vertices.emplace_back(Vertex(glm::vec3(x , y + cube_offset, z + cube_offset), glm::vec3(1, 1, block.faces[3]),normal));
                    mesh_data_.vertices.emplace_back(Vertex(glm::vec3(x + cube_offset, y + cube_offset, z + cube_offset), glm::vec3(0, 1, block.faces[3]),normal));
                    mesh_data_.vertices.emplace_back(Vertex(glm::vec3(x + cube_offset, y , z + cube_offset), glm::vec3(0, 0, block.faces[3]),normal));

                    mesh_data_.face_count++;
                }

                // Left
                bool draw_left_face = false;

                if (x == 0) {
                    if (chunk_left != nullptr) {
                        int index = CalculateIndex(k_chunk_size_ - 1, y, z);
                        if (chunk_left->operator[](index).is_air) {
                            draw_left_face = true;
                        }
                    }
                	else {
                		draw_left_face = true;
                	}
                }
                else {
                    int cube_left = CalculateIndex(x - 1, y, z);
                    if (cubes[cube_left].is_air) {
                        draw_left_face = true;
                    }
                }

                if (draw_left_face) {
                	auto normal = glm::u8vec3(-1, 0, 0);
                    mesh_data_.vertices.emplace_back(Vertex(glm::vec3(x , y + cube_offset, z + cube_offset), glm::vec3(0, 1, block.faces[4]),normal));
                    mesh_data_.vertices.emplace_back(Vertex(glm::vec3(x , y , z + cube_offset), glm::vec3(0, 0, block.faces[4]),normal));
                    mesh_data_.vertices.emplace_back(Vertex(glm::vec3(x , y , z ), glm::vec3(1, 0, block.faces[4]),normal));
                    mesh_data_.vertices.emplace_back(Vertex(glm::vec3(x , y + cube_offset, z ), glm::vec3(1, 1, block.faces[4]),normal));

                    mesh_data_.face_count++;
                }

                // Right
                bool draw_right_face = false;

                if (x == k_chunk_size_ - 1) {
                    if (chunk_right != nullptr) {
                        int index = CalculateIndex(0, y, z);
                        if (chunk_right->operator[](index).is_air) {
                            draw_right_face = true;
                        }
                    }
                	else {
                		draw_right_face = true;
                	}
                }
                else {
                    int cube_right = CalculateIndex(x + 1, y, z);
                    if (cubes[cube_right].is_air) {
                        draw_right_face = true;
                    }
                }

                if (draw_right_face) {
                	auto normal = glm::u8vec3(1,0, 0);
                    mesh_data_.vertices.emplace_back(Vertex(glm::vec3(x + cube_offset, y + cube_offset, z ), glm::vec3(0, 1, block.faces[5]),normal));
                    mesh_data_.vertices.emplace_back(Vertex(glm::vec3(x + cube_offset, y , z ), glm::vec3(0, 0, block.faces[5]),normal));
                    mesh_data_.vertices.emplace_back(Vertex(glm::vec3(x + cube_offset, y , z + cube_offset), glm::vec3(1, 0, block.faces[5]),normal));
                    mesh_data_.vertices.emplace_back(Vertex(glm::vec3(x + cube_offset, y + cube_offset, z + cube_offset), glm::vec3(1, 1, block.faces[5]),normal));

                    mesh_data_.face_count++;
                }
            }
        }
    }
    AddIndices();
}

int ChunkMesh::CalculateIndex(const int x, const int y, const int z) const {
	return (x * k_chunk_size_ * k_chunk_size_) + z * k_chunk_size_  + y;
}

void ChunkMesh::AddIndices() {
	std::array<float, 6> index_ordering = { 0, 1, 2, 2, 3, 0 };
	float vertex_index = 0;

	for (int x = 0; x < mesh_data_.face_count; x++) {
		for (unsigned int i = 0; i < 6; i++) {
			mesh_data_.indices.emplace_back(vertex_index + index_ordering[i]);
		}
		vertex_index += 4;
	}
}

void ChunkMesh::UploadToGPU() {
	gl_mesh_.vao.Bind();

	gl_mesh_.vbo.SetData(mesh_data_.vertices);
	gl_mesh_.ebo.SetData(mesh_data_.indices);

	gl_mesh_.vbo.Bind();
	gl_mesh_.ebo.Bind();

	gl_mesh_.vao.LinkAttrib(0,3,GL_UNSIGNED_SHORT, sizeof(Vertex), (void*)0);
	gl_mesh_.vao.LinkAttrib(1,3,GL_UNSIGNED_BYTE, sizeof(Vertex), (void*)offsetof(Vertex, tex_coords_and_texture));
	gl_mesh_.vao.LinkAttrib(2,3, GL_UNSIGNED_BYTE, sizeof(Vertex),(void*)offsetof(Vertex,normal));

	gl_mesh_.vao.Unbind();
	gl_mesh_.vbo.Unbind();
	gl_mesh_.ebo.Unbind();
}

void ChunkMesh::FreeGPUResources() {
	gl_mesh_.vao.Delete();
	gl_mesh_.vbo.Delete();
	gl_mesh_.ebo.Delete();
}

void ChunkMesh::RenderOpaque() {
	if (gl_mesh_.uploaded == false) {
		UploadToGPU();
		gl_mesh_.uploaded = true;
	}
	gl_mesh_.vao.Bind();
	glDrawElements(GL_TRIANGLES, mesh_data_.indices.size(), GL_UNSIGNED_INT, 0);
}



