//
// Created by svjat on 8/29/2025.
//

#include "ChunkMesh.h"
#include "World.h"

void ChunkMesh::GenerateMeshData( std::vector<CubeData>& cubes) {
    // Cubes are drawn off their back, bottom left corner



    for (int x = 0; x < k_chunk_size_; x++) {
		for (int z = 0; z < k_chunk_size_; z++) {
			for (int y = 0; y < k_chunk_size_; y++) {
				auto position = glm::u8vec3(x,y,z);

                int current_cube = CalculateIndex(x, y, z);
                if (cubes[current_cube].is_air) { continue; }

                Block block = SolidBlocks[cubes[current_cube].type];

                // Bottom
            	bool draw_bottom_face = false;

            	if (y == 0) {
            		int index = CalculateIndex(x, k_chunk_size_ - 1, z);
            		if (chunk_bottom_ != nullptr) {
            			if (chunk_bottom_->operator[](index).is_air) {
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
                	glm::u8 face = 0;
                	glm::u8 texture = block.faces[0];

                	std::array<glm::u8,4> ambient_occlusion_levels = CalculateAmbientOcclusionLevels(face,cubes,position);
                	AddMeshData(face, position, texture,ambient_occlusion_levels);
                }

				// Top
				bool draw_top_face = false;

				if (y == k_chunk_size_ - 1) {
					int index = CalculateIndex(x, 0, z);
					if (chunk_top_ != nullptr) {
						if (chunk_top_->operator[](index).is_air) {
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
					glm::u8 face = 1;
					glm::u8 texture = block.faces[1];

					std::array<glm::u8,4> ambient_occlusion_levels = CalculateAmbientOcclusionLevels(face,cubes,position);
					AddMeshData(face, position, texture, ambient_occlusion_levels);
				}

                // Back
                bool draw_back_face = false;

                if (z == 0) {
                    int index = CalculateIndex(x, y, k_chunk_size_- 1);
                    if (chunk_back_ != nullptr) {
                        if (chunk_back_->operator[](index).is_air) {
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
                	glm::u8 face = 2;
                	glm::u8 texture = block.faces[2];

                	std::array<glm::u8,4> ambient_occlusion_levels = CalculateAmbientOcclusionLevels(face,cubes,position);
                	AddMeshData(face, position, texture,ambient_occlusion_levels);
                }

                // Front
                bool draw_front_face = false;

                if (z == k_chunk_size_ - 1) {
                    if (chunk_front_ != nullptr) {
                        int index = CalculateIndex(x, y, 0);
                        if (chunk_front_->operator[](index).is_air) {
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
                	glm::u8 face = 3;
                	glm::u8 texture = block.faces[3];

                	std::array<glm::u8,4> ambient_occlusion_levels = CalculateAmbientOcclusionLevels(face,cubes,position);
                	AddMeshData(face, position, texture,ambient_occlusion_levels);
                }

                // Left
                bool draw_left_face = false;

                if (x == 0) {
                    if (chunk_left_ != nullptr) {
                        int index = CalculateIndex(k_chunk_size_ - 1, y, z);
                        if (chunk_left_->operator[](index).is_air) {
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
                	glm::u8 face = 4;
                	glm::u8 texture = block.faces[4];

                	std::array<glm::u8,4> ambient_occlusion_levels = CalculateAmbientOcclusionLevels(face,cubes,position);
                	AddMeshData(face, position, texture,ambient_occlusion_levels);
                }

                // Right
                bool draw_right_face = false;

                if (x == k_chunk_size_ - 1) {
                    if (chunk_right_ != nullptr) {
                        int index = CalculateIndex(0, y, z);
                        if (chunk_right_->operator[](index).is_air) {
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
                	glm::u8 face = 5;
                	glm::u8 texture = block.faces[5];

                	std::array<glm::u8,4> ambient_occlusion_levels = CalculateAmbientOcclusionLevels(face,cubes,position);
                	AddMeshData(face, position, texture,ambient_occlusion_levels);
                }
            }
        }
    }
    AddIndices();
	mesh_data_generated_ = true;
}

void ChunkMesh::CheckNearbyChunks(glm::ivec3 chunk_pos, World* world) {
	if (!found_all_neighbors_) {
		glm::ivec3 key_top = glm::ivec3(chunk_pos.x, chunk_pos.y + 1, chunk_pos.z);
		if (world->ChunkExists(key_top) && chunk_top_ == nullptr) {
			chunk_top_ = &world->getChunkData(key_top);
			mesh_data_generated_ = false;
		}

		glm::ivec3 key_bottom = glm::ivec3(chunk_pos.x, chunk_pos.y - 1, chunk_pos.z);
		if (world->ChunkExists(key_bottom) && chunk_bottom_ == nullptr) {
			chunk_bottom_ = &world->getChunkData(key_bottom);
			mesh_data_generated_ = false;
		}

		glm::ivec3 key_back = glm::ivec3(chunk_pos.x, chunk_pos.y, chunk_pos.z - 1);
		if (world->ChunkExists(key_back) && chunk_back_ == nullptr) {
			chunk_back_ = &world->getChunkData(key_back);
			mesh_data_generated_ = false;
		}

		glm::ivec3 key_front = glm::ivec3(chunk_pos.x,chunk_pos.y,chunk_pos.z + 1);
		if (world->ChunkExists(key_front) && chunk_front_ == nullptr) {
			chunk_front_ = &world->getChunkData(key_front);
			mesh_data_generated_ = false;
		}

		glm::ivec3 key_left = glm::ivec3(chunk_pos.x - 1, chunk_pos.y,chunk_pos.z);
		if (world->ChunkExists(key_left) && chunk_left_ == nullptr) {
			chunk_left_ = &world->getChunkData(key_left);
			mesh_data_generated_ = false;
		}

		glm::ivec3 key_right = glm::ivec3(chunk_pos.x + 1,chunk_pos.y,chunk_pos.z);
		if (world->ChunkExists(key_right) && chunk_right_ == nullptr) {
			chunk_right_ = &world->getChunkData(key_right);
			mesh_data_generated_ = false;
		}

		if (chunk_left_ != nullptr && chunk_right_ != nullptr
			&& chunk_back_ != nullptr && chunk_front_ != nullptr
			&& chunk_top_ != nullptr && chunk_bottom_ != nullptr) {
			found_all_neighbors_ = true;
		}
	}
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

	gl_mesh_.index_count = mesh_data_.indices.size();
}

void ChunkMesh::UploadToGPU() {
	if (!mesh_data_.vertices.empty()) {
		gl_mesh_.vao.GenerateID();
		gl_mesh_.vbo.GenerateID();
		gl_mesh_.ebo.GenerateID();

		gl_mesh_.vao.Bind();

		gl_mesh_.vbo.SetData(mesh_data_.vertices);
		gl_mesh_.ebo.SetData(mesh_data_.indices);

		gl_mesh_.vbo.Bind();
		gl_mesh_.ebo.Bind();

		gl_mesh_.vao.LinkAttrib(0,3,GL_UNSIGNED_BYTE,sizeof(Vertex),(void*)0);
		gl_mesh_.vao.LinkAttrib(1,3,GL_UNSIGNED_BYTE,sizeof(Vertex),(void*)offsetof(Vertex,tex_coords_and_texture));
		gl_mesh_.vao.LinkAttrib(2,1,GL_UNSIGNED_BYTE,sizeof(Vertex),(void*)offsetof(Vertex,normal));
		gl_mesh_.vao.LinkAttrib(3,1,GL_UNSIGNED_BYTE,sizeof(Vertex),(void*)offsetof(Vertex,ambient_occlusion));

		gl_mesh_.vao.Unbind();
		gl_mesh_.vbo.Unbind();
		gl_mesh_.ebo.Unbind();
	}
}

void ChunkMesh::FreeGPUResources() {
	if (!mesh_data_.vertices.empty()) {
		gl_mesh_.vao.Delete();
		gl_mesh_.vbo.Delete();
		gl_mesh_.ebo.Delete();
	}
}

void ChunkMesh::RenderOpaque() {
	if (!mesh_data_.vertices.empty()) {
		if (gl_mesh_.uploaded == false) {
			UploadToGPU();
			gl_mesh_.uploaded = true;
		}
		gl_mesh_.vao.Bind();
		glDrawElements(GL_TRIANGLES, gl_mesh_.index_count, GL_UNSIGNED_INT, nullptr);
	}
}

void ChunkMesh::AddMeshData(glm::u8 face, glm::u8vec3 position, glm::u8 texture, std::array<glm::u8,4> ambient_occlusion_level) {
	for (int i = face * 4; i < (face * 4) + 4; i++) {
		Vertex vertex{};

		auto mult = glm::u8vec3(4);
		vertex.position = (DefaultCubeData[i].position * mult) + (position * mult);
		vertex.tex_coords_and_texture = DefaultCubeData[i].tex_coords_and_texture + glm::u8vec3(0,0,texture);
		vertex.normal = DefaultCubeData[i].normal;
		vertex.ambient_occlusion = ambient_occlusion_level[i - (face * 4)];

		mesh_data_.vertices.emplace_back(vertex);
	}

	mesh_data_.face_count++;
}

std::array<glm::u8,4> ChunkMesh::CalculateAmbientOcclusionLevels(glm::u8 face, std::vector<CubeData>& cubes, glm::u8vec3 position) {
	std::array<glm::u8,4> out{0,0,0,0};
	std::array<bool, 9> blocks_exist{};

	int x_min = -1, y_min = -1, z_min = -1;
	int x_max = 1, y_max = 1, z_max = 1;

	switch (face) {
		case 0: { y_min = y_max = -1; break;}
		case 1: { y_min = y_max = 1; break;}
		case 2: { z_min = z_max = -1; break;}
		case 3: { z_min = z_max = 1; break;}
		case 4: { x_min = x_max = -1; break;}
		case 5: { x_min = x_max = 1; break;}
	}

	int i = 0;
	for (int x = x_min; x < x_max + 1; x++) {
		for (int y = y_min; y < y_max + 1; y++) {
			for (int z = z_min; z < z_max + 1; z++) {
				glm::i8vec3 index = glm::i8vec3(x, y, z) + glm::i8vec3(position);
				if (index.x < 0 || index.y < 0 || index.z < 0 || index.x > 31 || index.y > 31 || index.z > 31) {
					blocks_exist[i] = false;
				}
				else if (cubes[CalculateIndex(index.x,index.y,index.z)].is_air == false) {
					blocks_exist[i] = true;
				}
				else {
					blocks_exist[i] = false;
				}
				i++;
			}
		}
	}

	if (blocks_exist[1] and blocks_exist[3]) { out[0] = 3;}
	else if ((blocks_exist[0] and blocks_exist[3]) or (blocks_exist[0] and blocks_exist[1])) { out[0] = 2; }
	else if (blocks_exist[0] or blocks_exist[1] or blocks_exist[3]) { out[0] = 1; }
	else { out[0] = 0; }

	if (blocks_exist[3] and blocks_exist[7]) { out[1] = 3; }
	else if ((blocks_exist[3] and blocks_exist[6]) or (blocks_exist[7] and blocks_exist[6])) { out[1] = 2; }
	else if (blocks_exist[3] or blocks_exist[6] or blocks_exist[7]) { out[1] = 1; }
	else { out[1] = 0; }

	if (blocks_exist[5] and blocks_exist[7]) { out[2] = 3; }
	else if ((blocks_exist[7] and blocks_exist[8]) or (blocks_exist[8] and blocks_exist[5])) { out[2] = 2; }
	else if (blocks_exist[5] or blocks_exist[7] or blocks_exist[8]) { out[2] = 1; }
	else { out[2] = 0; }

	if (blocks_exist[1] and blocks_exist[5]) { out[3] = 3; }
	else if ((blocks_exist[1] and blocks_exist[2]) or (blocks_exist[2] and blocks_exist[5])) { out[3] = 2; }
	else if (blocks_exist[1] or blocks_exist[2] or blocks_exist[5]) { out[3] = 1; }
	else { out[3] = 0; }

	std::array<glm::u8,4> rotated;
	if (face == 3) {
		// Horizontal flip
		rotated = {out[1],out[0],out[3],out[2]};
		out = rotated;
	}
	if (face == 4) {
		// 90 degrees clockwise + horizontal flip
		rotated = {out[3],out[0],out[1],out[2],};
		out = rotated;
	}
	if (face == 5) {
		// 90 degrees clockwise + vertical flip
		rotated = {out[0],out[3],out[2], out[1]};
		out = rotated;
	}


	return out;
}






