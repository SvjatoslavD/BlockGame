//
// Created by svjat on 8/15/2025.
//

#include "TitleState.h"

#include "../application/Application.h"
#include "imgui-SFML.h"
#include "imgui.h"

TitleState::TitleState(StateManager* state_manager, Application* application):
   state_manager_(state_manager),application_(application) {
	assert(state_manager_ != nullptr);
	assert(application_ != nullptr);

	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
}

void TitleState::HandleInput(sf::Event& event) {
	ImGui::SFML::ProcessEvent(*application_->getWindow(), event);
}

void TitleState::Update(sf::Time delta_time) {
	sf::Vector2i mouse_pos = ImGui::GetMousePos();
	sf::Vector2f display_size = ImGui::GetIO().DisplaySize;
	ImGui::SFML::Update(mouse_pos,display_size,delta_time);
}

void TitleState::Draw() {
	ExampleWindow();
}

void TitleState::ExampleWindow() {
	ImGuiIO& io = ImGui::GetIO();
    float windowWidth = io.DisplaySize.x;
    float windowHeight = io.DisplaySize.y;

    // Create invisible window that covers the entire screen
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight));
    ImGui::Begin("Title Screen", nullptr,
                 ImGuiWindowFlags_NoTitleBar |
                 ImGuiWindowFlags_NoResize |
                 ImGuiWindowFlags_NoMove |
                 ImGuiWindowFlags_NoScrollbar |
                 ImGuiWindowFlags_NoCollapse |
                 ImGuiWindowFlags_NoBringToFrontOnFocus);

    // Title positioning
    const char* title = "BLOCK GAME";
    ImGui::PushFont(nullptr, 100.f); // Use default font, but you could load a bigger font

    // Calculate title size and center it
    ImVec2 titleSize = ImGui::CalcTextSize(title);
    float titlePosX = (windowWidth - titleSize.x) * 0.5f;
    float titlePosY = windowHeight * 0.25f; // 25% from top

	ImGui::SetCursorPos(ImVec2(titlePosX, titlePosY));

    // Make title text larger and colored
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f)); // White
    ImGui::Text(title);
    ImGui::PopStyleColor();
    ImGui::PopFont();

    // Button styling
    ImVec2 buttonSize = ImVec2(200, 50);
    float buttonSpacing = 20.0f;
    float totalButtonHeight = (buttonSize.y * 3) + (buttonSpacing * 2);
    float startY = (windowHeight - totalButtonHeight) * 0.5f + 100; // Center vertically, offset down
    float buttonPosX = (windowWidth - buttonSize.x) * 0.5f; // Center horizontally

	ImGui::SetWindowFontScale(1.5f);

    // Play Button
    ImGui::SetCursorPos(ImVec2(buttonPosX, startY));
    if (ImGui::Button("PLAY", buttonSize)) {
        HandlePlayButton();
    }

    // Options Button
    ImGui::SetCursorPos(ImVec2(buttonPosX, startY + buttonSize.y + buttonSpacing));
    if (ImGui::Button("OPTIONS", buttonSize)) {
        HandleOptionsButton();
    }

    // Exit Button
    ImGui::SetCursorPos(ImVec2(buttonPosX, startY + (buttonSize.y + buttonSpacing) * 2));
    if (ImGui::Button("EXIT", buttonSize)) {
        HandleExitButton();
    }

	// Reset font scale back to normal
	ImGui::SetWindowFontScale(1.0f);

    ImGui::End();
}

void TitleState::HandlePlayButton() {
	// state_manager_->ReplaceState(new WorldSelectState());
	std::cout << "Play button clicked!" << std::endl;
}

void TitleState::HandleOptionsButton() {
	// state_manager_->PushState(std::make_unique<OptionsState>(state_manager_, application_));
	std::cout << "Options button clicked!" << std::endl;
}

void TitleState::HandleExitButton() {
	application_->EndApplication();
	std::cout << "Exit button clicked!" << std::endl;
}
