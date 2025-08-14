//
// Created by svjat on 8/2/2025.
//

#ifndef GAME_H
#define GAME_H
#include <glm/vec3.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Window.hpp>

class GameState;

class StateManager {
public:
	StateManager(int width, int height, glm::vec3 position, sf::Vector2i window_center);
	~StateManager();

	void ChangeState(GameState* state);
	void PushState(GameState* state);
	void PopState();

	void HandleInput();
	void Update();
	void Draw();

	bool Running() const {return running;}
	void Quit() { running = false; }

	sf::Window* GetWindow() {return &window;}

private:
	sf::Window window;
	std::vector<GameState*> states;
	bool running;
};



#endif //GAME_H
