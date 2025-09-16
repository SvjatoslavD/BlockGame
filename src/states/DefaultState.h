//
// Created by svjat on 8/2/2025.
//

#ifndef DEFAULTSTATE_H
#define DEFAULTSTATE_H

#include "Application.h"

class DefaultState {
public:
	explicit DefaultState(StateManager* state_manager, Application* application):
		state_manager_(state_manager), application_(application) {};
	virtual ~DefaultState() = default;

	// Pausing the game will prevent game logic for progressing and inputs from registering
	// Can still render items is state above has transparency
	void Pause() { paused = true; };
	void Resume() { paused = false; };
	bool isPaused() const { return paused;}

	virtual void HandleEvents(sf::Event& event) {}
	virtual void HandleInput(float delta_time) {}
	virtual void Update(sf::Time delta_time) {}
	virtual void Draw() {}

protected:
	DefaultState() = default;
	Application* application_ = nullptr;
	StateManager* state_manager_ = nullptr;
	bool paused = false;
};

#endif //GAMESTATE_H
