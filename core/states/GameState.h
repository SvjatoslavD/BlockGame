//
// Created by svjat on 8/2/2025.
//

#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "../application/StateManager.h"

class GameState {
public:
	explicit GameState(StateManager* state_manager) : state_manager_(state_manager) {};
	virtual ~GameState() = default;

	// Pausing the game will prevent game logic for progressing and inputs from registering
	// Can still render items is state above has transparency
	void Pause() { paused = true; };
	void Resume() { paused = false; };
	bool isPaused() const { return paused;}

	virtual void HandleInput(StateManager* game, sf::Event& event) {};
	virtual void Update(StateManager* game) {};
	virtual void Draw(StateManager* game) {};

protected:
	GameState() {};
	StateManager* state_manager_;
	bool paused = false;
};

#endif //GAMESTATE_H
