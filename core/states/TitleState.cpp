//
// Created by svjat on 8/15/2025.
//

#include "TitleState.h"
#include "../application/Application.h"

TitleState::TitleState(StateManager* state_manager, Application* application):
   state_manager_(state_manager),application_(application) {
	assert(state_manager_ != nullptr);
	assert(application_ != nullptr);
}


void TitleState::HandleInput(sf::Event& event) {}

void TitleState::Update(sf::Time delta_time) {}

void TitleState::Draw() {}

void TitleState::ExampleWindow() {}
