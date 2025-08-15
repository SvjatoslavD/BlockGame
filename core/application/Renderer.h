//
// Created by svjat on 8/14/2025.
//

#ifndef RENDERER_H
#define RENDERER_H

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window.hpp>
#include <imgui.h>
#include <imgui-SFML.h>

class Application;

class Renderer {
public:
	Renderer();
	~Renderer();

	void Setup(Application* application);
	bool isSetup() const {return is_setup_;}
	void Use();

private:
	bool is_setup_ = false;

	Application* application_= nullptr;
	sf::Window* window_ = nullptr;

	void RenderSkybox();
	void RenderBlocks();
	void RenderEntities();
	void RenderUI();
};



#endif //RENDERER_H
