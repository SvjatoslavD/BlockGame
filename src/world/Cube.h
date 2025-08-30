//
// Created by svjat on 8/29/2025.
//

#ifndef CUBE_H
#define CUBE_H

#include <glm/glm.hpp>
#include "Tilemap.h"

struct Vertex {
	glm::u16vec3 position;
	glm::u8vec3 tex_coords_and_texture;
	glm::u8vec3 normal;
};

struct CubeData {
	bool is_air;
	glm::vec3 position;
	SolidBlockType type;
};

class Cube {

};



#endif //CUBE_H
