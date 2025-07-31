//
// Created by svjat on 7/30/2025.
//

#include "../header/Camera.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/quaternion_trigonometric.hpp>

Camera::Camera(int width, int height, glm::vec3 position, sf::Vector2i windowCenter) {
    this->width = width;
    this->height = height;
    this->Position = position;
    this->windowCenter = windowCenter;
}

void Camera::Matrix(float FOVdeg, float nearPlane, float farPlane, Shader& shader, const char* uniform) {
    glm::mat4 view;
    glm::mat4 projection;

    // model turns local coords into world coords (no change)
    glm::mat4 model = glm::mat4(1.0f);

    // turns world coords into 2d positions
    view = glm::lookAt(Position, Position + Orientation, Up);

    // defines how the points are seen on the screen
    projection = glm::perspective(glm::radians(FOVdeg), (float)width / height, nearPlane, farPlane);

    // pass all matrices to the shader
    shader.setMat4(uniform, (projection * view * model));
}

void Camera::Inputs(sf::Window& window, float deltaTime) {
    // handle events
    while (const std::optional event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }
        if (const auto* resized = event->getIf<sf::Event::Resized>()) {
            // adjust the viewport when the window is resized
            glViewport(0, 0, resized->size.x, resized->size.y);
        }
        if (const auto* mouseMoved = event->getIf<sf::Event::MouseMoved>()) {
            if (isButtonPressed(sf::Mouse::Button::Left)) {
                window.setMouseCursorVisible(false);
                float posX = static_cast<float>(mouseMoved->position.x);
                float posY = static_cast<float>(mouseMoved->position.y);

                if (firstMouse) {
                    posX = window.getSize().x / 2;
                    posY = window.getSize().y / 2;
                    firstMouse = false;
                }

                // some offsets - might be due to setting mouses position based on top - left coord
                float xoffset = posX - window.getSize().x / 2 + 8;
                float yoffset = window.getSize().y / 2 - 31 - posY; // reversed since y-coordinates go from bottom to top

                float sensitivity = 0.1f;
                xoffset *= sensitivity;
                yoffset *= sensitivity;

                yaw += xoffset;
                pitch += yoffset;

                // make sure that when pitch is out of bounds, screen doesn't get flipped
                if (pitch > 89.0f) {
                    pitch = 89.0f;
                };
                if (pitch < -89.0f) {
                    pitch = -89.0f;
                };

                glm::vec3 newOrientation;
                newOrientation.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
                newOrientation.y = sin(glm::radians(pitch));
                newOrientation.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
                Orientation = glm::normalize(newOrientation);

                sf::Mouse::setPosition(windowCenter);
            }
        }
        if (const auto* mouseButtonReleased = event->getIf<sf::Event::MouseButtonReleased>()) {
            if (mouseButtonReleased->button == sf::Mouse::Button::Left) {
                window.setMouseCursorVisible(true);
                firstMouse = true;
            }
        }
        // if (const auto* mouseWheel = event->getIf<sf::Event::MouseWheelScrolled>()) {
        //     if (mouseWheel->delta > 0) {
        //         fov -= mouseScrollSpeed * deltaTime;
        //         if (fov < 1.0f) {fov = 1.0f;}
        //     }
        //     if (mouseWheel->delta < 0) {
        //         fov += mouseScrollSpeed * deltaTime;
        //         if (fov > 55.0f) {fov = 55.0f;}
        //     }
        // }
    }

    // process input
    float cameraSpeed = 6.f * deltaTime;

    if (isKeyPressed(sf::Keyboard::Key::W)) {
        Position += cameraSpeed * Orientation;
    }
    if (isKeyPressed(sf::Keyboard::Key::S)) {
        Position -= cameraSpeed * Orientation;
    }
    if (isKeyPressed(sf::Keyboard::Key::A)) {
        Position -= glm::normalize(glm::cross(Orientation, Up)) * cameraSpeed;
    }
    if (isKeyPressed(sf::Keyboard::Key::D)) {
        Position += glm::normalize(glm::cross(Orientation, Up)) * cameraSpeed;
    }
    if (isKeyPressed(sf::Keyboard::Key::Space)) {
        Position += Up * cameraSpeed;
    }
    if (isKeyPressed(sf::Keyboard::Key::LShift)) {
        Position -= Up * cameraSpeed;
    }
    if (isKeyPressed(sf::Keyboard::Key::Q)) {
        window.close();
    }
}
