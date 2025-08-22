//
// Created by svjat on 7/30/2025.
//

#include "Camera.h"


Camera::Camera() = default;

void Camera::Setup(int width, int height, glm::vec3 position) {
	this->width_ = width;
	this->height_ = height;
	this->position_ = position;

	setup_ = true;
}

void Camera::Matrix(float FOV_deg, float near_plane, float far_plane, Shader &shader, const char *uniform) {
	assert(setup_);
	glm::mat4 view;
    glm::mat4 projection;

    // turns world coords into 2d positions
    view = glm::lookAt(position_, position_ + orientation_, up_);
    // defines how the points are seen on the screen
    projection = glm::perspective(glm::radians(FOV_deg), (float)width_ / height_, near_plane, far_plane);

    // pass all matrices to the shader
    glm::mat4 cameraMatrix = projection * view;
    shader.setMat4(uniform, cameraMatrix);
}

void Camera::ModelMatrix(Shader &shader, const char *uniform, glm::vec3 offset) {
	assert(setup_);
    // model turns local coords into world coords (no change)
    glm::mat4 model = glm::translate(glm::mat4(1.0f),offset);

    shader.setMat4(uniform, model);
}

void Camera::Update(glm::vec3 position, glm::vec3 orientation) {
	assert(setup_);
	position_ = position;
	orientation_ = orientation;
}
