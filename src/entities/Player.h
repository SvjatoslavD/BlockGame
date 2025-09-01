//
// Created by svjat on 8/21/2025.
//

#ifndef PLAYER_H
#define PLAYER_H

#include "Camera.h"
#include <glm\vec3.hpp>

class Application;

class Player {
public:
	Player() = default;
	~Player() = default;

	void Setup(Application* application, float health, glm::vec3 position);
	void HandleInput(sf::Event& event);
	void Update(float delta_time, Shader* our_shader);

	glm::vec3 getGlobalCoordinates() const { return position_;};
	[[nodiscard]] float getHealth() const { return health_;};
	glm::vec3 getChunkCoordinates() const;
	Camera* getCamera() { return &camera_;};

private:
	float health_;

	glm::vec3 position_ = glm::vec3(0.0f);
	glm::vec3 velocity_ = glm::vec3(0.0f);
	glm::ivec2 chunk_coordinates_;
	glm::vec3 orientation_ = glm::vec3(-1.0f, 0.0f, 0.0f);

	glm::vec3 up_ = glm::vec3(0.0f, 1.0f, 0.0f);
	sf::Vector2i window_center_;

	bool first_mouse_ = true;
	float speed_boost_ = 1.f;
	float yaw_ = -90.0f;
	float pitch_ = 0.0f;

	Camera camera_;
	Application* application_;
};



#endif //PLAYER_H
