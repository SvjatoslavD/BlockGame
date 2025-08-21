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

	Shader our_shader("../../resources/shaders/default.vert", "../../resources/shaders/default.frag");
	World our_world(1);
	Texture test("../../assets/images/tile-map.png", GL_TEXTURE_2D_ARRAY, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE, 16);
	test.TexUnit(our_shader, "TextureArray1", 0);
	// Player player_(location);
}

void GameState::HandleInput(sf::Event& event) {
	if (!paused) {
		ImGui::SFML::ProcessEvent(*application_->getWindow(), event);
	}
}

void GameState::Update(sf::Time delta_time) {
	if (!paused) {
		sf::Vector2i mouse_pos = ImGui::GetMousePos();
		sf::Vector2f display_size = ImGui::GetIO().DisplaySize;
		ImGui::SFML::Update(mouse_pos,display_size,delta_time);
	}
}

void GameState::Draw() {
	//     ourShader.Activate();
	//     glActiveTexture(GL_TEXTURE0);
	//     test.Bind();
	//     our_world.Update(camera.getChunkCoordinates());
	//     our_world.RenderChunks(ourShader, camera);
}