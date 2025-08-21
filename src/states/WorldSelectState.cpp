//
// Created by svjat on 8/17/2025.
//

#include "WorldSelectState.h"

#include "Application.h"
#include "GameState.h"
#include "imgui-SFML.h"
#include "imgui.h"

WorldSelectState::WorldSelectState(StateManager* state_manager, Application* application) :
	DefaultState(state_manager, application) {
}

void WorldSelectState::HandleInput(sf::Event& event) {
	if (!paused) {
		ImGui::SFML::ProcessEvent(*application_->getWindow(), event);
	}
}

void WorldSelectState::Update(sf::Time delta_time) {
	if (!paused) {
		sf::Vector2i mouse_pos = ImGui::GetMousePos();
		sf::Vector2f display_size = ImGui::GetIO().DisplaySize;
		ImGui::SFML::Update(mouse_pos,display_size,delta_time);
	}
}

void WorldSelectState::Draw() {
	WorldSelectWindow();
}

void WorldSelectState::WorldSelectWindow() {
    ImGuiIO& io = ImGui::GetIO();
    float window_width = io.DisplaySize.x;
    float window_height = io.DisplaySize.y;

	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);

    // Create invisible window that covers the entire screen
    ImGui::Begin("World Select Screen", nullptr,
                 ImGuiWindowFlags_NoTitleBar |
                 ImGuiWindowFlags_NoResize |
                 ImGuiWindowFlags_NoMove |
                 ImGuiWindowFlags_NoScrollbar |
                 ImGuiWindowFlags_NoCollapse |
                 ImGuiWindowFlags_NoBringToFrontOnFocus);

    // Title positioning
    const char* title = "SELECT WORLD";
    ImGui::PushFont(nullptr); // Use default font

    // Calculate title size and center it
	ImGui::SetWindowFontScale(2.0f);
    ImVec2 title_size = ImGui::CalcTextSize(title);
    float title_pos_x = (window_width  * 0.5f) - title_size.x * 0.5f;
    float title_pos_y = window_height * 0.05f; // 10% from top

    ImGui::SetCursorPos(ImVec2(title_pos_x, title_pos_y));

    // Make title text colored
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f)); // White
    ImGui::Text(title);
    ImGui::SetWindowFontScale(1.0f);
    ImGui::PopStyleColor();
    ImGui::PopFont();

    // World list area
    float list_width = window_width - (window_width * .1f);
    float list_height = window_height * 0.7f;
    float list_pos_x = (window_width - list_width) * 0.5f;
    float list_pos_y = title_pos_y + window_height * 0.05f;

    ImGui::SetCursorPos(ImVec2(list_pos_x, list_pos_y));

    // Create child window for world list
    ImGui::BeginChild("WorldList", ImVec2(list_width, list_height), true,
                     ImGuiWindowFlags_NoScrollWithMouse);

    // Example worlds (replace with actual world loading)
    static std::vector<WorldInfo> worlds_ = {
        {"My World", "Creative", "2025-08-17 14:30:22", "Version 0.1", true},
        {"Survival Island", "Survival", "2025-08-16 09:15:33", "Version 0.1", false},
        {"Test World", "Creative", "2025-08-15 20:45:12", "Version 0.1", false}
    };

    static int selected_world_ = -1;

    for (int i = 0; i < worlds_.size(); i++) {
        ImGui::PushID(i);

        bool is_selected = (selected_world_ == i);
        if (ImGui::Selectable("", is_selected, ImGuiSelectableFlags_AllowDoubleClick, ImVec2(0, 80))) {
            selected_world_ = i;

            // Double click to play world
            if (ImGui::IsMouseDoubleClicked(0)) {
                PlaySelectedWorld(i);
            }
        }

        // Draw world info on the selectable area
        ImVec2 p = ImGui::GetItemRectMin();
        ImVec2 p_max = ImGui::GetItemRectMax();
        ImDrawList* draw_list = ImGui::GetWindowDrawList();

        // Background color for selected item
        if (is_selected) {
            draw_list->AddRectFilled(p, p_max, IM_COL32(70, 70, 70, 255));
        }

        // World icon placeholder
        draw_list->AddRectFilled(ImVec2(p.x + 5, p.y + 5), ImVec2(p.x + 65, p.y + 65), IM_COL32(100, 150, 100, 255));
        draw_list->AddText(ImVec2(p.x + 25, p.y + 25), IM_COL32(255, 255, 255, 255), "W");

        // World name
        draw_list->AddText(ImVec2(p.x + 75, p.y + 5), IM_COL32(255, 255, 255, 255), worlds_[i].name.c_str());

        // Game mode and last played
        std::string info = worlds_[i].game_mode + " - Last played: " + worlds_[i].last_played;
        draw_list->AddText(ImVec2(p.x + 75, p.y + 25), IM_COL32(180, 180, 180, 255), info.c_str());

        // Version info
        draw_list->AddText(ImVec2(p.x + 75, p.y + 45), IM_COL32(150, 150, 150, 255), worlds_[i].version.c_str());

        // Cheats indicator
        if (worlds_[i].cheats_enabled) {
            draw_list->AddText(ImVec2(p.x + 200, p.y + 45), IM_COL32(255, 255, 0, 255), "(Cheats)");
        }

        ImGui::PopID();
    }

    ImGui::EndChild();

    // Button styling
    ImVec2 button_size = ImVec2(160, 40);
    float button_spacing = (window_width - (button_size.x * 5) - window_width * 0.1f)/4;
    float button_start_x = (window_width * 0.1f * 0.5f);
    float button_y = list_pos_y + list_height + 40.0f;

    ImGui::SetWindowFontScale(1.2f);

    // Play Selected button
    bool can_play = (selected_world_ >= 0);
    if (!can_play) {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.6f, 0.6f, 0.6f, 1.0f));
    }

    ImGui::SetCursorPos(ImVec2(button_start_x, button_y));
    if (ImGui::Button("PLAY SELECTED", button_size) && can_play) {
        PlaySelectedWorld(selected_world_);
    }

    if (!can_play) {
        ImGui::PopStyleColor(4);
    }

    // Create New World button
    ImGui::SetCursorPos(ImVec2(button_start_x + button_size.x + button_spacing, button_y));
    if (ImGui::Button("CREATE NEW", button_size)) {
        CreateNewWorld();
    }

    // Edit button
    bool can_edit = (selected_world_ >= 0);
    if (!can_edit) {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.6f, 0.6f, 0.6f, 1.0f));
    }

    ImGui::SetCursorPos(ImVec2(button_start_x + (button_size.x + button_spacing) * 2, button_y));
    if (ImGui::Button("EDIT", button_size) && can_edit) {
        EditWorld(selected_world_);
    }

    if (!can_edit) {
        ImGui::PopStyleColor(4);
    }

    // Delete button
    bool can_delete = (selected_world_ >= 0);
    if (!can_delete) {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.6f, 0.6f, 0.6f, 1.0f));
    }

    ImGui::SetCursorPos(ImVec2(button_start_x + (button_size.x + button_spacing) * 3, button_y));
    if (ImGui::Button("DELETE", button_size) && can_delete) {
        DeleteWorld(selected_world_);
    }

    if (!can_delete) {
        ImGui::PopStyleColor(4);
    }

    // Back button
    ImGui::SetCursorPos(ImVec2(button_start_x + (button_size.x + button_spacing) * 4, button_y));
    if (ImGui::Button("BACK", button_size)) {
	    BackToTitle();
    }

    // Reset font scale back to normal
    ImGui::SetWindowFontScale(1.0f);

    ImGui::End();
}

// Button functionality methods (add these to your WorldSelectState class)
void WorldSelectState::PlaySelectedWorld(int world_index) {
    // TODO: Load the selected world and transition to GameState
    // For now, just print to console
    std::cout << "Playing world: " << world_index << std::endl;

    state_manager_->ReplaceState(Lookup::GameState);
}

void WorldSelectState::CreateNewWorld() {
    // TODO: Open world creation dialog or transition to WorldCreateState
    std::cout << "Creating new world..." << std::endl;

    // Example transition to world creation state:
    // state_manager_->PushState(std::make_unique<WorldCreateState>(state_manager_, application_));
}

void WorldSelectState::EditWorld(int worldIndex) {
    // TODO: Open world editing dialog
    std::cout << "Editing world: " << worldIndex << std::endl;

    // Could open a popup or transition to edit state
}

void WorldSelectState::DeleteWorld(int worldIndex) {
    // TODO: Show confirmation dialog and delete world
    std::cout << "Deleting world: " << worldIndex << std::endl;

    // Example confirmation dialog:
    // show_delete_confirmation = true;
    // world_to_delete = worldIndex;
}

void WorldSelectState::BackToTitle() {
    // Return to title screen
    state_manager_->ReplaceState(Lookup::TitleState);
}