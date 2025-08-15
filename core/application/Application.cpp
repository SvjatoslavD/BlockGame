//
// Created by svjat on 8/13/2025.
//

#include "Application.h"


Application::Application(const unsigned int win_width, const unsigned int win_height, unsigned int target_fps):
	win_width_(win_width), win_height_(win_height), render_delta_time_(1.f/static_cast<float>(target_fps)) {
	// initialize members
	SetupWindow();
	renderer_.Setup(this);
	state_manager_.Setup(this);

	// state_manager_.PushState(TitleState);
}

Application::~Application() = default;

void Application::StartApplication() { GameLoop(); }

void Application::SetupWindow() {
	// load variables needed for the creation of the window
	sf::ContextSettings settings;
	settings.majorVersion = 3;
	settings.minorVersion = 3;
	settings.depthBits = 24;

	// create the window
	window_.create(sf::VideoMode({ win_width_, win_height_ }), "OpenGL", sf::Style::Default, sf::State::Windowed, settings);

	// set window as target of opengl
	bool active = window_.setActive();
	if (!active) {
		std::cerr << "Failed to set active window" << std::endl;
	}
	glViewport(0, 0, static_cast<int>(win_width_), static_cast<int>(win_height_));

	// initialize glew
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		std::cerr << "Failed to initialize GLEW" << std::endl;
	}

	// set some openGl options
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);
}

void Application::GameLoop() {
	// can have different tick speeds for rendering and logic
	sf::Clock clock;
	float delta_time = 0.0f;

	float last_logic_frame = 0.0f;
	float last_render_frame = 0.0f;

	while (is_running_) {
		// calculate delta time per frame
		delta_time = clock.restart().asSeconds();

		last_logic_frame += delta_time;
		last_render_frame += delta_time;

		state_manager_.HandleInput();

		// logic
		if (last_logic_frame >= k_logic_delta_time_) {
			last_logic_frame -= k_logic_delta_time_;

			state_manager_.Update(delta_time);
		}

		// rendering
		if (last_render_frame >= render_delta_time_) {
			last_render_frame -= render_delta_time_;

			state_manager_.Draw();
		}
	}
}

