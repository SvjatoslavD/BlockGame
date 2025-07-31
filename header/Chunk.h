//
// Created by svjat on 7/27/2025.
//

#ifndef CHUNK_H
#define CHUNK_H

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <vector>
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

class Chunk {
public:
	explicit Chunk(unsigned int chunkSize);
	~Chunk();

	void RenderChunk();

private:
	int tileSize = 16;
	VAO VAO1;
	VBO VBO1;
	EBO EBO1;
	unsigned int chunkSize;
	unsigned int triangleCount;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	void GenerateChunk();
	void GenerateFaces();
};
#endif //CHUNK_H
