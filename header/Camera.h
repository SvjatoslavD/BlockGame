//
// Created by svjat on 7/30/2025.
//

#ifndef CAMERA_H
#define CAMERA_H
#include <SFML/Window/Window.hpp>
#include <glm/vec3.hpp>

#include "Shader.h"

class Camera {
public:
    Camera(int width, int height, glm::vec3 position, sf::Vector2i window_center);

    void Matrix(float FOV_deg, float near_plane, float far_plane, Shader& shader, const char* uniform);
    void Inputs(sf::Window& window, float delta_time);
private:
    glm::vec3 position_;
    glm::vec3 orientation_ = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 up_ = glm::vec3(0.0f, 1.0f, 0.0f);

    int width_, height_;

    float speed_ = 1.0f;
    float sensitivity_ = 100.0f;
    bool first_mouse_ = true;
    float yaw_ = -90.0f;
    float pitch_ = 0.0f;
    sf::Vector2i window_center_;
};
#endif // CAMERA_H
