//
// Created by svjat on 8/14/2025.
//

#include "rendering/Renderer.h"

#include <iostream>

#include "core/Application.h"
#include "gl/glew.h"

#include "imgui-SFML.h"

Renderer::Renderer() = default;

Renderer::~Renderer() {
};

void Renderer::Setup(sf::Window* window) {
	window_ = window;

	bool imgui_sfml_init = ImGui::SFML::Init(*window_,static_cast<sf::Vector2f>(window_->getSize()));
	if (!imgui_sfml_init) {
		std::cerr << "Failed to initialize ImGui-SFML" << std::endl;
	}

	is_setup_ = true;
}

void Renderer::Clear() {
	glClearColor(0.5f, 0.8f, 0.92f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::Display() {
	ImGui::SFML::Render();
	window_->display();
}