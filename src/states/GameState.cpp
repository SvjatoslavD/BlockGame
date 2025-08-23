//
// Created by svjat on 8/21/2025.
//

#include "GameState.h"

#include "Application.h"
#include "Texture.h"
#include "World.h"
#include "imgui-SFML.h"
#include "imgui.h"

GameState::GameState(StateManager* state_manager, Application* application) :
	DefaultState(state_manager, application) {

	our_shader_.Setup("../../resources/shaders/default.vert", "../../resources/shaders/default.frag");
	world_.Setup(1);
	test_.Setup("../../assets/images/tile-map.png", GL_TEXTURE_2D_ARRAY, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE, 16);
	test_.TexUnit(our_shader_, "TextureArray1", 0);
	player_.Setup(application, 10.f, glm::vec3(0,150,0));
}

void GameState::HandleInput(sf::Event& event) {
	if (!paused) {
		if (const auto* resized = event.getIf<sf::Event::Resized>()) {
			// adjust the viewport when the window is resized
			glViewport(0, 0, resized->size.x, resized->size.y);
		}

		ImGui::SFML::ProcessEvent(*application_->getWindow(), event);

		player_.HandleInput(event);
	}
}

void GameState::Update(sf::Time delta_time) {
	if (!paused) {
		sf::Vector2i mouse_pos = ImGui::GetMousePos();
		sf::Vector2f display_size = ImGui::GetIO().DisplaySize;
		ImGui::SFML::Update(mouse_pos,display_size,delta_time);

		player_.Update(delta_time.asSeconds(), &our_shader_);
		world_.Update(player_.getChunkCoordinates());
	}
}

void GameState::Draw() {
	our_shader_.Activate();
	glActiveTexture(GL_TEXTURE0);
	test_.Bind();
	world_.RenderChunks(our_shader_, *player_.getCamera());
}
