//
// Created by svjat on 7/27/2025.
//

#include "../include/Chunk.h"

#include <vector>

Chunk::Chunk(unsigned int chunkSize) {
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	unsigned int vertexIndex = 0;
	for (unsigned int x = 0; x < chunkSize; x++) {
		for (unsigned int y = 0; y < chunkSize; y++) {
			for (unsigned int z = 0; z < chunkSize; z++) {
				// Top
				0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
				0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			   -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
			   -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,

				0,1,2,2,3,0,		// Top
			}
		}
	}
}
