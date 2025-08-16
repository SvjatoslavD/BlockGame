//
// Created by svjat on 8/14/2025.
//

#ifndef RENDERER_H
#define RENDERER_H

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window.hpp>

class Application;

class Renderer {
public:
	Renderer();
	~Renderer();

	void Setup(Application* application);
	bool isSetup() const {return is_setup_;}
	void Clear();
	void Display();

private:
	bool is_setup_ = false;

	Application* application_= nullptr;
	sf::Window* window_ = nullptr;
};



#endif //RENDERER_H
