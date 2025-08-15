//
// Created by svjat on 8/2/2025.
//

#ifndef GAME_H
#define GAME_H

#include <SFML/Window/Window.hpp>
#include <stack>

#include "Renderer.h"

class Application;
class GameState;

class StateManager {
public:
	StateManager();
	~StateManager();

	void Setup(Application* application);

	void ReplaceState(GameState* state);
	void PushState(GameState* state);
	void PopState();
	unsigned int getStatesSize() const { return states_.size();}

	void HandleInput();
	void Update(float delta_time);
	void Draw();

private:
	bool state_manager_setup_ = false;
	std::stack<GameState*> states_;

	Application* application_;
	Renderer* renderer_;
	sf::Window* window_;
};



#endif //GAME_H
