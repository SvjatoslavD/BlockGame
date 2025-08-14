//
// Created by svjat on 8/13/2025.
//
#include <gtest/gtest.h>
#include "../core/application/Application.h"

TEST(Application, WindowCreation) {
	int win_width = 1000;
	int win_height = 800;
	Application app(win_width,win_height, 60);

	ASSERT_EQ(app.GetWindowSize(),sf::Vector2u(win_width,win_height));
}