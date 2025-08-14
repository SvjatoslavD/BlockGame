//
// Created by svjat on 8/13/2025.
//

#ifndef APPLICATION_H
#define APPLICATION_H

#include <GL/glew.h>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Window.hpp>
#include <iostream>

#include "StateManager.h"

class Application {
public:
	Application(unsigned int win_width, unsigned int win_height, unsigned int target_fps);
	~Application();

	void EndApplication() {is_running_ = false;};
	sf::Vector2u GetWindowSize() const { return window_.getSize();};

private:
	bool is_running_ = true;
	unsigned int win_width_, win_height_;
	const float k_logic_delta_time_ = 1.0f / 60.0f;
	float render_delta_time_;

	void GameLoop();
	void SetupWindow();

	// StateManager state_manager_;
	// ThreadManager thread_manager_;
	// Renderer renderer_;
	// ResourceManager resource_manager_;
	sf::Window window_;
};



#endif //APPLICATION_H
