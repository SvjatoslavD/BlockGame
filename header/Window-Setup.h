//
// Created by svjat on 7/23/2025.
//

#ifndef WINDOWSETUP_H
#define WINDOWSETUP_H

#include <SFML/Window.hpp>
#include "GL/glew.h"

class WindowSetup {
public:
    WindowSetup();
    ~WindowSetup();
    sf::Window Start(int win_width, int win_height);
};

#endif // WINDOWSETUP_H
