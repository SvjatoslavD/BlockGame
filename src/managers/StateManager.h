//
// Created by svjat on 8/2/2025.
//

#ifndef GAME_H
#define GAME_H

#include <SFML/Window/Window.hpp>
#include <iostream>
#include <stack>

#include "../rendering/Renderer.h"

class Application;
class DefaultState;
namespace Lookup {
	enum State;
}

class StateManager {
public:
	StateManager();
	~StateManager();

	void Setup(Application* application);

	void ReplaceState(Lookup::State lookup);
	void PushState(Lookup::State lookup);
	void PopState();
	unsigned int getStatesSize() const { return states_.size();}

	void HandleInput();
	void Update(sf::Time delta_time);
	void Draw();

private:
	bool state_manager_setup_ = false;
	std::stack<DefaultState*> states_;

	Application* application_;
	Renderer* renderer_;
	sf::Window* window_;

	void CreateNewState(Lookup::State lookup);
};

namespace Lookup {
	enum State {
		TitleState,
		WorldSelectState,
		GameState,
	};
};


#endif //GAME_H
