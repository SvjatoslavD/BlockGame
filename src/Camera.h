//
// Created by svjat on 7/30/2025.
//

#ifndef CAMERA_H
#define CAMERA_H

#include <SFML/Window/Window.hpp>
#include <glm/vec3.hpp>
#include <glm/ext.hpp>

#include "opengl/Shader.h"

class Camera {
public:
    Camera();

	void Setup(int width, int height, glm::vec3 position);
    void Matrix(float FOV_deg, float near_plane, float far_plane, Shader& shader, const char* uniform);
    void ModelMatrix(Shader &shader, const char *uniform, glm::vec3 offset);

    void Update(glm::vec3 position, glm::vec3 orientation);

private:
	bool setup_ = false;
	int width_, height_;

	glm::vec3 position_;
	glm::vec3 orientation_;
	glm::vec3 up_ = glm::vec3(0.0f, 1.0f, 0.0f);

    sf::Vector2i window_center_;
};
#endif // CAMERA_H
