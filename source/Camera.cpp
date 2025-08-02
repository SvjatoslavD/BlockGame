//
// Created by svjat on 7/30/2025.
//

#include "../header/Camera.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/quaternion_trigonometric.hpp>

Camera::Camera(int width, int height, glm::vec3 position, sf::Vector2i window_center) {
    this->width_ = width;
    this->height_ = height;
    this->position_ = position;
    this->window_center_ = window_center;
}

void Camera::Matrix(float FOV_deg, float near_plane, float far_plane, Shader &shader, const char *uniform) {
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
    // model turns local coords into world coords (no change)
    glm::mat4 model = glm::translate(glm::mat4(1.0f),offset);

    shader.setMat4(uniform, model);
    glm::vec4 test = model * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
}

void Camera::HandleInputs(sf::Window& window, float delta_time) {
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

                if (first_mouse_) {
                    posX = window.getSize().x / 2;
                    posY = window.getSize().y / 2;
                    first_mouse_ = false;
                }

                // some offsets - might be due to setting mouses position based on top - left coord
                float xoffset = posX - window.getSize().x / 2 + 8;
                float yoffset = window.getSize().y / 2 - 31 - posY; // reversed since y-coordinates go from bottom to top

                float sensitivity = 0.1f;
                xoffset *= sensitivity;
                yoffset *= sensitivity;

                yaw_ += xoffset;
                pitch_ += yoffset;

                // make sure that when pitch is out of bounds, screen doesn't get flipped
                if (pitch_ > 89.0f) {
                    pitch_ = 89.0f;
                };
                if (pitch_ < -89.0f) {
                    pitch_ = -89.0f;
                };

                glm::vec3 newOrientation;
                newOrientation.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
                newOrientation.y = sin(glm::radians(pitch_));
                newOrientation.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
                orientation_ = glm::normalize(newOrientation);

                sf::Mouse::setPosition(window_center_);
            }
        }
        if (const auto* mouseButtonReleased = event->getIf<sf::Event::MouseButtonReleased>()) {
            if (mouseButtonReleased->button == sf::Mouse::Button::Left) {
                window.setMouseCursorVisible(true);
                first_mouse_ = true;
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
    float cameraSpeed = 15.f * delta_time;

    if (isKeyPressed(sf::Keyboard::Key::W)) {
        position_ += cameraSpeed * orientation_;
    }
    if (isKeyPressed(sf::Keyboard::Key::S)) {
        position_ -= cameraSpeed * orientation_;
    }
    if (isKeyPressed(sf::Keyboard::Key::A)) {
        position_ -= glm::normalize(glm::cross(orientation_, up_)) * cameraSpeed;
    }
    if (isKeyPressed(sf::Keyboard::Key::D)) {
        position_ += glm::normalize(glm::cross(orientation_, up_)) * cameraSpeed;
    }
    if (isKeyPressed(sf::Keyboard::Key::Space)) {
        position_ += up_ * (cameraSpeed/2);
    }
    if (isKeyPressed(sf::Keyboard::Key::LShift)) {
        position_ -= up_ * (cameraSpeed/2);
    }
    if (isKeyPressed(sf::Keyboard::Key::Q)) {
        window.close();
    }

    // After finishing moving the player
    CalculateChunkCoordinates();
}

void Camera::CalculateChunkCoordinates() {
    chunk_coordinates_ = glm::vec2(floor(position_.x/16),floor(position_.z/16));
}
