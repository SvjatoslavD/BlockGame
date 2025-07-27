//
// Created by svjat on 7/23/2025.
//

#include "../include/WindowSetup.h"

WindowSetup::WindowSetup() {

}


sf::Window WindowSetup::start(int win_width, int win_height) {
    // load variables needed for the creation of the window
    sf::ContextSettings settings;
    settings.majorVersion = 3;
    settings.minorVersion = 3;
    settings.depthBits = 24;

    // create the window
    sf::Window window(sf::VideoMode({(unsigned)win_width, (unsigned)win_height}), "OpenGL", sf::Style::Default, sf::State::Windowed, settings);
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60);
    window.setMouseCursorVisible(false);

    // initialize the OpenGL states
    window.setActive();
    glViewport(0,0,win_width,win_height);
    glewExperimental = GL_TRUE;
    glewInit();

    return window;
}
