//
// Created by svjat on 8/13/2025.
//
#include <gtest/gtest.h>
#include "ApplicationFixture.h"

TEST_F(ApplicationFixture, WindowCreation) {
	win_width = 1000;
	win_height = 800;

	ASSERT_EQ(ApplicationFixture::application_default_->getWindowSize(),sf::Vector2u(win_width,win_height));
	ASSERT_EQ(ApplicationFixture::application_high_->getWindowSize(),sf::Vector2u(win_width*2,win_height*2));
}

TEST_F(ApplicationFixture, RendererSetup) {
	Renderer renderer;
	ASSERT_EQ(renderer.isSetup(),false);

	renderer.Setup(ApplicationFixture::application_default_);

	ASSERT_EQ(renderer.isSetup(),true);
}

TEST_F(ApplicationFixture, StateManagerSetup) {

}