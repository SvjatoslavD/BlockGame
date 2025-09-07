//
// Created by svjat on 8/29/2025.
//

#include "ChunkMesh.h"
#include "World.h"

void ChunkMesh::GenerateMeshData(std::vector<CubeData>& cubes) {
	if (create_mesh and !mesh_created) {
		mesh_data_.vertices.clear();
		mesh_data_.indices.clear();
		mesh_data_.face_count = 0;

		// Cubes are drawn off their back, bottom left corner
		GenerateMainMesh(cubes);
		GenerateMeshCorners(cubes);

		AddIndices();

		gl_mesh_.index_count = mesh_data_.indices.size();
		gl_mesh_.uploaded = false;

		mesh_created = true;
	}
}


void ChunkMesh::CheckNearbyChunks(glm::ivec3 chunk_pos, World* world) {
	if (world_ == nullptr) {
		world_ = world;
		chunk_position_ = chunk_pos;
	}

	int dist = world_->getRenderDistance();
	for (int i = 0; i < 6; i++) {
		if (!neighbor_chunks_found_[i]) {
			if (world_->ChunkExists(chunk_position_ + neighbor_chunks_position_[i])) { neighbor_chunks_found_[i] = true; }
			else if (chunk_pos.y <= -dist && i == 0) {
				neighbor_chunks_found_[i] = true;
			}
			else if (chunk_pos.y >= dist && i == 1) {
				neighbor_chunks_found_[i] = true;
			}
			else if (chunk_pos.z <= -dist && i == 2) {
				neighbor_chunks_found_[i] = true;
			}
			else if (chunk_pos.z >= dist && i == 3) {
				neighbor_chunks_found_[i] = true;
			}
			else if (chunk_pos.x <= -dist && i == 4) {
				neighbor_chunks_found_[i] = true;
			}
			else if (chunk_pos.x >= dist && i == 5) {
				neighbor_chunks_found_[i] = true;
			}
		}
	}

	bool found_all_neighbors = true;
	for (int i = 0; i < 6; i++) {
		if (!neighbor_chunks_found_[i]) {
			found_all_neighbors = false;
		}
	}

	if (found_all_neighbors) {
		create_mesh = true;
	}
}

int ChunkMesh::CalculateIndex(const int x, const int y, const int z) const {
	return (x * k_chunk_size_ * k_chunk_size_) + z * k_chunk_size_  + y;
}

void ChunkMesh::AddIndices() {
	std::array<float, 6> index_ordering = { 0, 1, 2, 2, 3, 0 };

	for (int x = 0; x < mesh_data_.face_count; x++) {
		for (unsigned int i = 0; i < 6; i++) {
			mesh_data_.indices.emplace_back(vertex_index_ + index_ordering[i]);
		}

		vertex_index_ += 4;
	}
}

void ChunkMesh::UploadToGPU() {
	if (gl_mesh_.ids_created == false) {
		gl_mesh_.vbo.GenerateID();
		gl_mesh_.ebo.GenerateID();
		gl_mesh_.vao.GenerateID();
		gl_mesh_.ids_created = true;
	}

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
				if (GetNeighboringBlock(index, cubes).is_air == false) {
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
	if (face == 0) {
		// Rotate clockwise
		rotated = {out[3], out[2], out[1],out[0], };
		out = rotated;
	}
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

CubeData ChunkMesh::GetNeighboringBlock(glm::i8vec3 block_position, std::vector<CubeData>& cubes) {
	if (block_position.x >= 0 && block_position.x < 32 &&
	   block_position.y >= 0 && block_position.y < 32 &&
	   block_position.z >= 0 && block_position.z < 32) {
		return cubes[CalculateIndex(block_position.x, block_position.y, block_position.z)];
	   }

	// Determine which neighbor chunk we need
	glm::ivec3 target_chunk_pos = chunk_position_;
	glm::i8vec3 neighbor_pos = block_position;

	// X axis neighbors
	if (block_position.x < 0) {
		target_chunk_pos += glm::ivec3(-1,0,0);
		neighbor_pos.x = 31;  // Wrap to far side of neighbor chunk
	} else if (block_position.x >= 32) {
		target_chunk_pos += glm::ivec3(1,0,0);
		neighbor_pos.x = 0;   // Wrap to near side of neighbor chunk
	}

	// Y axis neighbors
	if (block_position.y < 0) {
		target_chunk_pos += glm::ivec3(0,-1, 0);
		neighbor_pos.y = 31;
	} else if (block_position.y >= 32) {
		target_chunk_pos += glm::ivec3(0,1, 0);
		neighbor_pos.y = 0;
	}

	// Z axis neighbors
	if (block_position.z < 0) {
		target_chunk_pos += glm::ivec3(0,0,-1);
		neighbor_pos.z = 31;
	} else if (block_position.z >= 32) {
		target_chunk_pos += glm::ivec3(0,0, 1);
		neighbor_pos.z = 0;
	}

	// If neighbor chunk exists, get the block from it
	if (world_->ChunkExists(target_chunk_pos) == true) {
		std::vector<CubeData>& target_chunk = world_->getChunkData(target_chunk_pos);
		return target_chunk[CalculateIndex(neighbor_pos.x,neighbor_pos.y,neighbor_pos.z)];
	}

	// If no neighbor chunk, return default (e.g., air block or solid block)
	CubeData default_block;
	default_block.is_air = true;  // Assume air outside loaded chunks
	return default_block;
};

void ChunkMesh::GenerateMainMesh(std::vector<CubeData>& cubes) {
	for (int x = 0; x < k_chunk_size_; x++) {
		for (int z = 0; z < k_chunk_size_; z++) {
			for (int y = 0; y < k_chunk_size_; y++) {
				auto position = glm::u8vec3(x,y,z);

				int current_cube = CalculateIndex(x, y, z);
				if (cubes[current_cube].is_air) { continue; }

				Block block = SolidBlocks[cubes[current_cube].type];

				// Bottom
				if (y > 0) {
					int cube_bottom = CalculateIndex(x, y - 1, z);
					if (cubes[cube_bottom].is_air) {
						glm::u8 face = 0;
						glm::u8 texture = block.faces[0];

						std::array<glm::u8,4> ambient_occlusion_levels = CalculateAmbientOcclusionLevels(face,cubes,position);
						AddMeshData(face, position, texture,ambient_occlusion_levels);
					}
				}

				// Top
				if (y <= k_chunk_size_ - 2) {
					int cube_top = CalculateIndex(x, y + 1, z);
					if (cubes[cube_top].is_air) {
						glm::u8 face = 1;
						glm::u8 texture = block.faces[1];

						std::array<glm::u8,4> ambient_occlusion_levels = CalculateAmbientOcclusionLevels(face,cubes,position);
						AddMeshData(face, position, texture, ambient_occlusion_levels);
					}
				}

				// Back
				if (z > 0) {
					int cube_back = CalculateIndex(x, y, z - 1);
					if (cubes[cube_back].is_air) {
						glm::u8 face = 2;
						glm::u8 texture = block.faces[2];

						std::array<glm::u8,4> ambient_occlusion_levels = CalculateAmbientOcclusionLevels(face,cubes,position);
						AddMeshData(face, position, texture,ambient_occlusion_levels);
					}
				}

				// Front
				if (z <= k_chunk_size_ - 2) {
					int cube_front = CalculateIndex(x, y, z + 1);
					if (cubes[cube_front].is_air) {
						glm::u8 face = 3;
						glm::u8 texture = block.faces[3];

						std::array<glm::u8,4> ambient_occlusion_levels = CalculateAmbientOcclusionLevels(face,cubes,position);
						AddMeshData(face, position, texture,ambient_occlusion_levels);
					}
				}

				// Left
				if (x > 0) {
					int cube_left = CalculateIndex(x - 1, y, z);
					if (cubes[cube_left].is_air) {
						glm::u8 face = 4;
						glm::u8 texture = block.faces[4];

						std::array<glm::u8,4> ambient_occlusion_levels = CalculateAmbientOcclusionLevels(face,cubes,position);
						AddMeshData(face, position, texture,ambient_occlusion_levels);
					}
				}

				// Right
				if (x <= k_chunk_size_ - 2) {
					int cube_right = CalculateIndex(x + 1, y, z);
					if (cubes[cube_right].is_air) {
						glm::u8 face = 5;
						glm::u8 texture = block.faces[5];

						std::array<glm::u8,4> ambient_occlusion_levels = CalculateAmbientOcclusionLevels(face,cubes,position);
						AddMeshData(face, position, texture,ambient_occlusion_levels);
					}
				}
			}
		}
	}
}

void ChunkMesh::GenerateMeshCorners(std::vector<CubeData>& cubes) {
	// Bottom
	for (int x = 0; x < k_chunk_size_; x++) {
		for (int z = 0; z < k_chunk_size_; z++) {
			auto position = glm::i8vec3(x,0,z);

			int current_cube = CalculateIndex(x, 0, z);
			if (cubes[current_cube].is_air) { continue; }

			Block block = SolidBlocks[cubes[current_cube].type];

			bool draw_bottom_face = false;

			if (GetNeighboringBlock(position + glm::i8vec3(0,-1,0),cubes).is_air == true) {
				draw_bottom_face = true;
			}
			else if (chunk_position_.y == -world_->getRenderDistance()) {
				draw_bottom_face = true;
			}
			if (draw_bottom_face) {
				glm::u8 face = 0;
				glm::u8 texture = block.faces[0];

				std::array<glm::u8,4> ambient_occlusion_levels = CalculateAmbientOcclusionLevels(face,cubes,position);
				AddMeshData(face, position, texture,ambient_occlusion_levels);
			}
		}
	}

	// Top
	for (int x = 0; x < k_chunk_size_; x++) {
		for (int z = 0; z < k_chunk_size_; z++) {
			auto position = glm::i8vec3(x,k_chunk_size_ - 1,z);

			int current_cube = CalculateIndex(x, k_chunk_size_ - 1, z);
			if (cubes[current_cube].is_air) { continue; }

			Block block = SolidBlocks[cubes[current_cube].type];

			bool draw_top_face = false;

			if (GetNeighboringBlock(position + glm::i8vec3(0,1,0),cubes).is_air == true) {
				draw_top_face = true;
			}
			else if (chunk_position_.y == world_->getRenderDistance()) {
				draw_top_face = true;
			}

			if (draw_top_face) {
				glm::u8 face = 1;
				glm::u8 texture = block.faces[1];

				std::array<glm::u8,4> ambient_occlusion_levels = CalculateAmbientOcclusionLevels(face,cubes,position);
				AddMeshData(face, position, texture, ambient_occlusion_levels);
			}
		}
	}

	// Back
	for (int x = 0; x < k_chunk_size_; x++) {
		for (int y = 0; y < k_chunk_size_; y++) {
			auto position = glm::i8vec3(x,y, 0);

			int current_cube = CalculateIndex(x,y,0);
			if (cubes[current_cube].is_air) { continue; }

			Block block = SolidBlocks[cubes[current_cube].type];

			bool draw_back_face = false;

			if (GetNeighboringBlock(position + glm::i8vec3(0,0,-1),cubes).is_air == true) {
				draw_back_face = true;
			}
			else if (chunk_position_.z == -world_->getRenderDistance()) {
				draw_back_face = true;
			}

			if (draw_back_face) {
				glm::u8 face = 2;
				glm::u8 texture = block.faces[2];

				std::array<glm::u8,4> ambient_occlusion_levels = CalculateAmbientOcclusionLevels(face,cubes,position);
				AddMeshData(face, position, texture,ambient_occlusion_levels);
			}
		}
	}


	// Front
	for (int x = 0; x < k_chunk_size_; x++) {
		for (int y = 0; y < k_chunk_size_; y++) {
			auto position = glm::i8vec3(x,y, k_chunk_size_ - 1);

			int current_cube = CalculateIndex(x,y,k_chunk_size_ - 1);
			if (cubes[current_cube].is_air) { continue; }

			Block block = SolidBlocks[cubes[current_cube].type];

			bool draw_front_face = false;

			if (GetNeighboringBlock(position + glm::i8vec3(0,0,1),cubes).is_air == true) {
				draw_front_face = true;
			}
			else if (chunk_position_.z == world_->getRenderDistance()) {
				draw_front_face = true;
			}

			if (draw_front_face) {
				glm::u8 face = 3;
				glm::u8 texture = block.faces[3];

				std::array<glm::u8,4> ambient_occlusion_levels = CalculateAmbientOcclusionLevels(face,cubes,position);
				AddMeshData(face, position, texture,ambient_occlusion_levels);
			}
		}
	}

	// Left
	for (int y = 0; y < k_chunk_size_; y++) {
		for (int z = 0; z < k_chunk_size_; z++) {
			auto position = glm::i8vec3(0, y, z);

			int current_cube = CalculateIndex(0,y,z);
			if (cubes[current_cube].is_air) { continue; }

			Block block = SolidBlocks[cubes[current_cube].type];

			bool draw_left_face = false;

			if (GetNeighboringBlock(position + glm::i8vec3(-1,0,0),cubes).is_air == true) {
				draw_left_face = true;
			}
			else if (chunk_position_.x == -world_->getRenderDistance()) {
				draw_left_face = true;
			}
			if (draw_left_face) {
				glm::u8 face = 4;
				glm::u8 texture = block.faces[4];

				std::array<glm::u8,4> ambient_occlusion_levels = CalculateAmbientOcclusionLevels(face,cubes,position);
				AddMeshData(face, position, texture,ambient_occlusion_levels);
			}
		}
	}

	// Right
	for (int y = 0; y < k_chunk_size_; y++) {
		for (int z = 0; z < k_chunk_size_; z++) {
			auto position = glm::i8vec3(k_chunk_size_ - 1, y, z);

			int current_cube = CalculateIndex(k_chunk_size_ - 1,y,z);
			if (cubes[current_cube].is_air) { continue; }

			Block block = SolidBlocks[cubes[current_cube].type];

			bool draw_right_face = false;

			if (GetNeighboringBlock(position + glm::i8vec3(1,0,0),cubes).is_air == true) {
				draw_right_face = true;
			}
			else if (chunk_position_.x == world_->getRenderDistance()) {
				draw_right_face = true;
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






