// //
// // Created by svjat on 5/17/2025.
// //
//
#include "StateManager.h"
#include "../states/GameState.h"
#include "Application.h"

#include <iostream>
#include <SFML/Window.hpp>

StateManager::StateManager() = default;

StateManager::~StateManager() {
	while (!states_.empty()) {
		delete states_.top();
		states_.pop();
	}
}

void StateManager::Setup(Application* application) {
	application_ = application;
	renderer_ = application->getRenderer();
	window_ = application->getWindow();

	state_manager_setup_ = true;
}

void StateManager::ReplaceState(GameState* state) {
	if (!states_.empty()) {
		delete states_.top();
		states_.pop();
	}
	states_.push(state);
}

void StateManager::PushState(GameState* state) {
	if (!states_.empty()) {
		states_.top()->Pause();
	}
	states_.push(state);
}

void StateManager::PopState() {
	if (!states_.empty()) {
		delete states_.top();
		states_.pop();
		states_.top()->Resume();
	}
	else {
		std::cerr << "No state to pop" << std::endl;
	}
}

void StateManager::HandleInput() {
	assert(state_manager_setup_);
	while (auto event = window_->pollEvent()) {
		// events/inputs which can be used in all states
		if (event->is<sf::Event::Closed>()) { application_->EndApplication(); }
		if (event->is<sf::Event::KeyPressed>()) {}

		if (!states_.empty()) {
			// events/inputs specific to the state
			states_.top()->HandleInput(this, *event);
		}

		#ifdef _DEBUG
		if (isKeyPressed(sf::Keyboard::Key::Q)) {application_->EndApplication();}
		#endif
	}
}

void StateManager::Update(float delta_time) {
	assert(state_manager_setup_);
	if (!states_.empty()) {
		// Calculate physics

		// Update player

		// Update entities
	}
}

void StateManager::Draw() {
	assert(state_manager_setup_);
	// Should notify the renderer to draw the appropriate objects
	renderer_->Use();
}


