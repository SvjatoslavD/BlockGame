//
// Created by svjat on 8/2/2025.
//

#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "../application/GameManager.h"

class GameState {
public:
	explicit GameState(GameManager* game) : game(game) {};
	virtual ~GameState() = default;

	void Pause() { paused = true; };
	void Resume() { paused = false; };
	bool IsPaused() const { return paused;}

	virtual void HandleInput(GameManager* game, sf::Event& event) {};
	virtual void Update(GameManager* game) {};
	virtual void Draw(GameManager* game) {};

protected:
	GameState() {};
	GameManager* game;
	bool paused = false;
};

#endif //GAMESTATE_H
