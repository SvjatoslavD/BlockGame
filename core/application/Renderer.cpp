//
// Created by svjat on 8/14/2025.
//

#include "Application.h"
#include "Renderer.h"

Renderer::Renderer() = default;

Renderer::~Renderer() {
	ImGui::SFML::Shutdown();
};

void Renderer::Setup(Application* application) {
	application_ = application;
	window_ = application_->getWindow();

	bool created = ImGui::SFML::Init(*window_,static_cast<sf::Vector2f>(window_->getSize()));
	if (!created) {
		std::cerr << "Failed to initialize ImGui Window" << std::endl;
	}

	is_setup_ = true;
}

void Renderer::Use() {
	glClearColor(0.5f, 0.8f, 0.92f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	RenderSkybox();
	RenderBlocks();
	RenderEntities();
	RenderUI();

	window_->display();
}

void Renderer::RenderSkybox() {}

void Renderer::RenderBlocks() {}

void Renderer::RenderEntities() {}

void Renderer::RenderUI() {}