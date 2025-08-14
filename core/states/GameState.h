//
// Created by svjat on 8/2/2025.
//

#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "../application/StateManager.h"

class GameState {
public:
	explicit GameState(StateManager* game) : game(game) {};
	virtual ~GameState() = default;

	void Pause() { paused = true; };
	void Resume() { paused = false; };
	bool IsPaused() const { return paused;}

	virtual void HandleInput(StateManager* game, sf::Event& event) {};
	virtual void Update(StateManager* game) {};
	virtual void Draw(StateManager* game) {};

protected:
	GameState() {};
	StateManager* game;
	bool paused = false;
};

#endif //GAMESTATE_H
