// //
// // Created by svjat on 5/17/2025.
// //
//
#include "StateManager.h"

#include "TitleState.h"
#include "WorldSelectState.h"
#include "GameState.h"

#include "Application.h"

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

void StateManager::ReplaceState(Lookup::State lookup) {
	if (!states_.empty()) {
		delete states_.top();
		states_.pop();
	}
	CreateNewState(lookup);
}

void StateManager::PushState(Lookup::State lookup) {
	if (!states_.empty()) {
		states_.top()->Pause();
	}
	CreateNewState(lookup);
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

void StateManager::HandleInput(float delta_time) {
	assert(state_manager_setup_);
	states_.top()->HandleInput(delta_time);
	while (auto event = window_->pollEvent()) {
		// events/inputs which can be used in all states
		if (event->is<sf::Event::Closed>()) { application_->EndApplication(); }

		if (!states_.empty()) {
			// events/inputs specific to the state
			states_.top()->HandleEvents(*event);
		}

		#ifdef _DEBUG
		if (isKeyPressed(sf::Keyboard::Key::Q)) {application_->EndApplication();}
		#endif
	}
}

void StateManager::Update(sf::Time delta_time) {
	assert(state_manager_setup_);
	if (!states_.empty()) {
		states_.top()->Update(delta_time);
	}
}

void StateManager::Draw() {
	assert(state_manager_setup_);
	if (!states_.empty()) {
		renderer_->Clear();
		states_.top()->Draw();
		renderer_->Display();
	}
}

void StateManager::CreateNewState(Lookup::State lookup) {
	assert(state_manager_setup_);
	switch (lookup) {
		case Lookup::TitleState: states_.push(new TitleState(this, application_)); break;
		case Lookup::WorldSelectState: states_.push(new WorldSelectState(this,application_)); break;
		case Lookup::GameState: states_.push(new GameState(this, application_)); break;
		default: std::cerr << "Unknown lookup" << std::endl; break;
	}
}


