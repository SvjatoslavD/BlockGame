//
// Created by svjat on 8/13/2025.
//

#include "Application.h"


Application::Application(const unsigned int win_width, const unsigned int win_height, unsigned int target_fps):
	win_width_(win_width), win_height_(win_height), render_delta_time_(1/target_fps) {
	// initialize members
	SetupWindow();

	// start game loop
	GameLoop();
}

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
	glViewport(0, 0, win_width_, win_height_);

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

Application::~Application() {}

void Application::GameLoop() {
	// can have different tick speeds for rendering and logic
	sf::Clock clock;
	float last_frame = 0.0f;
	float delta_time = 0.0f;

	float last_logic_frame = 0.0f;
	float last_render_frame = 0.0f;

	while (is_running_) {
		// calculate delta time per frame
		float current_frame = clock.getElapsedTime().asSeconds();
		delta_time = current_frame - last_frame;
		last_logic_frame += delta_time;
		last_render_frame += delta_time;
		last_frame = current_frame;

		// logic
		if (last_logic_frame >= k_logic_delta_time_) {
			last_logic_frame -= k_logic_delta_time_;


		}

		// rendering
		if (last_render_frame >= render_delta_time_) {
			last_render_frame -= render_delta_time_;


			glClearColor(0.5f, 0.8f, 0.92f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			window_.display();
		}

		if (clock.getElapsedTime().asSeconds() > 5) {
			EndApplication();
		}
	}
}

