//
// Created by svjat on 7/24/2025.
//

#ifndef GAMEVARIABLES_H
#define GAMEVARIABLES_H

// Move these variables to a game class at some point

#include <glm/glm.hpp>
#include "VBO.h"
// camera
glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f, 5.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);

#endif //GAMEVARIABLES_H
