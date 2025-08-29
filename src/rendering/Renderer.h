//
// Created by svjat on 8/14/2025.
//

#ifndef RENDERER_H
#define RENDERER_H

#include <../../cmake-build-debug-visual-studio/vcpkg_installed/x64-windows/include/SFML/Graphics/RenderWindow.hpp>
#include <../../cmake-build-debug-visual-studio/vcpkg_installed/x64-windows/include/SFML/Window.hpp>

class Application;

class Renderer {
public:
	Renderer();
	~Renderer();

	void Setup(sf::Window* window);
	bool isSetup() const {return is_setup_;}
	void Clear();
	void Display();

private:
	bool is_setup_ = false;

	sf::Window* window_;
};



#endif //RENDERER_H
