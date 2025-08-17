//
// Created by svjat on 8/17/2025.
//

#ifndef WORLDSELECTSTATE_H
#define WORLDSELECTSTATE_H

#include "GameState.h"

class WorldSelectState: public GameState {
public:
	explicit WorldSelectState(StateManager* state_manager, Application* application);
	~WorldSelectState() override = default;

	void HandleInput(sf::Event& event) override;
	void Update(sf::Time delta_time) override;
	void Draw() override;

private:
	void WorldSelectWindow();

	void PlaySelectedWorld(int world_index);
	void CreateNewWorld();
	void EditWorld(int world_index);
	void DeleteWorld(int world_index);
	void BackToTitle();
};

// Supporting data structure (add to your header file)
struct WorldInfo {
	std::string name;
	std::string game_mode;  // "Creative", "Survival", "Adventure", etc.
	std::string last_played;
	std::string version;
	bool cheats_enabled;
};

#endif //WORLDSELECTSTATE_H
