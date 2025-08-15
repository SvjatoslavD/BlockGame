//
// Created by svjat on 8/15/2025.
//

#ifndef TITLESTATE_H
#define TITLESTATE_H

#include "GameState.h"

class Application;

class TitleState : public GameState {
public:
	explicit TitleState(StateManager* state_manager, Application* application);
	~TitleState() override = default;

	void HandleInput(sf::Event& event) override;
	void Update(sf::Time delta_time) override;
	void Draw() override;

private:
	Application* application_ = nullptr;
	StateManager* state_manager_ = nullptr;

	void ExampleWindow();
};



#endif //TITLESTATE_H
