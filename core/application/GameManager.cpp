// //
// // Created by svjat on 5/17/2025.
// //
//
// #include "GameManager.h"
//
// #include <iostream>
// #include <SFML/Window.hpp>
//
// GameManager::GameManager(int width, int height, glm::vec3 position, sf::Vector2i window_center) {
// 	window.create(sf::VideoMode({850, 975}), "Minesweeper", sf::Style::Default);
// 	window.setPosition({500, 0});
// 	window.setFramerateLimit(30);
//
// 	running = true;
//
// 	// ChangeState(new TitleState(this));
// }
//
// GameManager::~GameManager() {
// 	while (!states.empty()) {
// 		delete states.back();
// 		states.pop_back();
// 	}
// 	window.close();
// }
//
// void GameManager::ChangeState(GameState* state) {
// 	if (!states.empty()) {
// 		delete states.back();
// 		states.pop_back();
// 	}
// 	states.push_back(state);
// }
//
// void GameManager::PushState(GameState* state) {
// 	if (!states.empty()) {
// 		states.back()->Pause();
// 	}
// 	states.push_back(state);
// }
//
// void GameManager::PopState() {
// 	if (!states.empty()) {
// 		delete states.back();
// 		states.pop_back();
// 		states.back()->Resume();
// 	}
// 	else {
// 		std::cerr << "No state to pop" << std::endl;
// 	}
// }
//
// void GameManager::HandleInput() {
// 	while (auto event = window.pollEvent()) {
// 		if (event->is<sf::Event::Closed>()) { running = false; }
// 		if (event->is<sf::Event::KeyPressed>()) {}
//
// 		states.back()->HandleInput(this, *event);
// 	}
//
// 	if (isKeyPressed(sf::Keyboard::Key::Q)) {running = false;}
// }
//
// void GameManager::Update() {
// 	states.back()->Update(this);
// }
//
// void GameManager::Draw() {
// 	window.clear();
// 	states.back()->Draw(this);
// 	window.display();
// }
//
//
