//
// Created by svjat on 7/27/2025.
//

#ifndef CHUNK_H
#define CHUNK_H
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

class Chunk {
public:
	Chunk(unsigned int chunkSize);
	~Chunk();

	void GenerateChunk();
	void RenderChunk();

private:
	unsigned int VAO, VBO, EBO;
	unsigned int chunkSize;
	unsigned int triangleCount;
};

struct Vertex {
	glm::vec3 position;
	glm::vec2 texCoord;
	// glm::vec3 normal;
};
#endif //CHUNK_H
