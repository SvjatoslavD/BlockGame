//
// Created by svjat on 8/21/2025.
//

#include "Player.h"
#include "Application.h"

void Player::Setup(Application* application, float health, glm::vec3 position) {
	application_ = application;
	health_ = health;
	position_ = position;

	sf::Window* window = application->getWindow();
	int win_width = window->getSize().x;
	int win_height = window->getSize().y;

	window_center_ = (window->getPosition() + sf::Vector2i(window->getSize().x / 2, window->getSize().y / 2));
	camera_.Setup(win_width, win_height, glm::vec3(0.f, 110.f, 0.f));
}

void Player::HandleInputAndEvents(sf::Event& event) {
	sf::Window* window = application_->getWindow();

	if (const auto* mouseMoved = event.getIf<sf::Event::MouseMoved>()) {
        if (isButtonPressed(sf::Mouse::Button::Left)) {
            window->setMouseCursorVisible(false);
            float posX = static_cast<float>(mouseMoved->position.x);
            float posY = static_cast<float>(mouseMoved->position.y);

            if (first_mouse_) {
                posX = window->getSize().x / 2;
                posY = window->getSize().y / 2;
                first_mouse_ = false;
            }

            // some offsets - might be due to setting mouses position based on top - left coord
            float xoffset = posX - window->getSize().x / 2 + 8;
            float yoffset = window->getSize().y / 2 - 31 - posY; // reversed since y-coordinates go from bottom to top

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
    if (const auto* mouseButtonReleased = event.getIf<sf::Event::MouseButtonReleased>()) {
        if (mouseButtonReleased->button == sf::Mouse::Button::Left) {
            window->setMouseCursorVisible(true);
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


    if (isKeyPressed(sf::Keyboard::Key::LControl)) { speed_boost_ = 4.f; }
	else { speed_boost_ = 1.f;}

	velocity_ = glm::vec3(0.0f);

    if (isKeyPressed(sf::Keyboard::Key::W)) {
        velocity_ += orientation_;
    }
    if (isKeyPressed(sf::Keyboard::Key::S)) {
        velocity_ -= orientation_;
    }
    if (isKeyPressed(sf::Keyboard::Key::A)) {
        velocity_ -= glm::normalize(glm::cross(orientation_, up_));
    }
    if (isKeyPressed(sf::Keyboard::Key::D)) {
        velocity_ += glm::normalize(glm::cross(orientation_, up_));
    }
    if (isKeyPressed(sf::Keyboard::Key::Space)) {
        velocity_ += up_ * .5f;
    }
    if (isKeyPressed(sf::Keyboard::Key::LShift)) {
        velocity_ -= up_ * .5f;
    }
};

void Player::Update(float delta_time, Shader* our_shader) {
	if (velocity_ != glm::vec3(0.0f)) {
		position_ += velocity_ *  40.f * speed_boost_ * delta_time;
	}

	camera_.Update(position_, orientation_);
	camera_.Matrix(50.0f, 0.1f, 10000.f, *our_shader, "cameraMatrix");
}

glm::vec3 Player::getChunkCoordinates() const {
	auto out = glm::vec3(0);

	out.x = floor(position_.x/32);
	out.y = floor(position_.y/32);
	out.z = floor(position_.z/32);

	return out;
}

