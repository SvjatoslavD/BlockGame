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
    glm::vec3 Position;
    glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);

    int width, height;

    float speed = 1.0f;
    float sensitivity = 100.0f;
    bool firstMouse = true;
    float yaw = -90.0f;
    float pitch = 0.0f;
    sf::Vector2i windowCenter;

    Camera(int width, int height, glm::vec3 position, sf::Vector2i localWindowCenter);

    void Matrix(float FOVdeg, float nearPlane, float farPlane, Shader& shader, const char* uniform);
    void Inputs(sf::Window& window, float deltaTime);
};
#endif // CAMERA_H
