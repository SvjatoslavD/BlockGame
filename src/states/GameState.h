//
// Created by svjat on 8/2/2025.
//

#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "Application.h"

class GameState {
public:
	explicit GameState(StateManager* state_manager, Application* application):
		state_manager_(state_manager), application_(application) {};
	virtual ~GameState() = default;

	// Pausing the game will prevent game logic for progressing and inputs from registering
	// Can still render items is state above has transparency
	void Pause() { paused = true; };
	void Resume() { paused = false; };
	bool isPaused() const { return paused;}

	virtual void HandleInput(sf::Event& event) {}
	virtual void Update(sf::Time delta_time) {}
	virtual void Draw() {}

protected:
	GameState() = default;
	Application* application_ = nullptr;
	StateManager* state_manager_ = nullptr;
	bool paused = false;
};

#endif //GAMESTATE_H
