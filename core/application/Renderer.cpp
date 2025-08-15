//
// Created by svjat on 8/14/2025.
//

#include "Application.h"
#include "Renderer.h"

Renderer::Renderer() = default;

Renderer::~Renderer() {
};

void Renderer::Setup(Application* application) {
	application_ = application;
	window_ = application_->getWindow();

	is_setup_ = true;
}

void Renderer::Clear() {
	glClearColor(0.5f, 0.8f, 0.92f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::Display() {
	window_->display();
}