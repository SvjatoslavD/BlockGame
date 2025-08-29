//
// Created by svjat on 8/21/2025.
//

#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "DefaultState.h"
#include "Player.h"
#include "Texture.h"
#include "World.h"

class StateManager;
class Application;

class GameState : public DefaultState {
public:
	explicit GameState(StateManager* state_manager, Application* application);

	void HandleInput(sf::Event& event) override;
	void Update(sf::Time delta_time) override;
	void Draw() override;
	~GameState() override = default;

private:
	void Gameplay();

	Player player_;
	Shader default_shader_;
	Shader lighting_shader_;
	World world_;
	Texture test_;
};



#endif //GAMESTATE_H
