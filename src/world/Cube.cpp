//
// Created by svjat on 8/29/2025.
//

#include "Cube.h"

#include <array>

std::array<Vertex,24> DefaultCubeData {
	// Bottom
	Vertex(glm::u8vec3(0,0,1), glm::u8vec3(1, 0, 0),0,0),
	Vertex(glm::u8vec3(1,0,1), glm::u8vec3(0, 0, 0),0,0),
	Vertex(glm::u8vec3(1,0,0), glm::u8vec3(0, 1, 0),0,0),
	Vertex(glm::u8vec3(0,0,0), glm::u8vec3(1, 1, 0),0,0),
	// Top
	Vertex(glm::u8vec3(0,1,0), glm::u8vec3(1, 1, 0),1,0),
	Vertex(glm::u8vec3(1,1,0), glm::u8vec3(0, 1, 0),1,0),
	Vertex(glm::u8vec3(1,1,1), glm::u8vec3(0, 0, 0),1,0),
	Vertex(glm::u8vec3(0,1,1), glm::u8vec3(1, 0, 0),1,0),
	// Back
	Vertex(glm::u8vec3(0,0,0), glm::u8vec3(0, 0, 0),2,0),
	Vertex(glm::u8vec3(1,0,0), glm::u8vec3(1, 0, 0),2,0),
	Vertex(glm::u8vec3(1,1,0), glm::u8vec3(1, 1, 0),2,0),
	Vertex(glm::u8vec3(0,1,0), glm::u8vec3(0, 1, 0),2,0),
	// Front
	Vertex(glm::u8vec3(1,0,1), glm::u8vec3(0, 0, 0),3,0),
	Vertex(glm::u8vec3(0,0,1), glm::u8vec3(1, 0, 0),3,0),
	Vertex(glm::u8vec3(0,1,1), glm::u8vec3(1, 1, 0),3,0),
	Vertex(glm::u8vec3(1,1,1), glm::u8vec3(0, 1, 0),3,0),
	// Left
	Vertex(glm::u8vec3(0,0,1), glm::u8vec3(0, 0, 0),4,0),
	Vertex(glm::u8vec3(0,0,0), glm::u8vec3(1, 0, 0),4,0),
	Vertex(glm::u8vec3(0,1,0), glm::u8vec3(1, 1, 0),4,0),
	Vertex(glm::u8vec3(0,1,1), glm::u8vec3(0, 1, 0),4,0),
	// Right
	Vertex(glm::u8vec3(1,0,0), glm::u8vec3(0, 0, 0),5,0),
	Vertex(glm::u8vec3(1,0,1), glm::u8vec3(1, 0, 0),5,0),
	Vertex(glm::u8vec3(1,1,1), glm::u8vec3(1, 1, 0),5,0),
	Vertex(glm::u8vec3(1,1,0), glm::u8vec3(0, 1, 0),5,0),
};