cmake_minimum_required(VERSION 3.30)
project(BlockGame)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_SOURCE_DIR ${CMAKE_BINARY_DIR})

find_package(SFML COMPONENTS Network Graphics Window Audio System CONFIG REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm CONFIG REQUIRED)

set(SOURCE
        main.cpp
        core/opengl/Shader.cpp
        core/Window-Setup.cpp
        core/opengl/EBO.cpp
        core/opengl/VAO.cpp
        core/opengl/VBO.cpp
        core/opengl/Texture.cpp
        core/Chunk.cpp
        core/Camera.cpp
        core/World.cpp
        core/opengl/Shader.h
        core/Window-Setup.h
        core/opengl/EBO.h
        core/opengl/VAO.h
        core/opengl/VBO.h
        core/opengl/Texture.h
        core/Chunk.h
        core/Camera.h
        core/World.h
        external/FastNoiseLite.h
        core/application/GameManager.cpp
        core/application/GameManager.h
        core/states/GameState.h
        core/Tilemap.cpp
        core/Tilemap.h
)

add_executable(main ${SOURCE})

target_link_libraries(main PRIVATE
        GLEW::GLEW
        SFML::Network SFML::Graphics SFML::Window SFML::Audio SFML::System
        glm::glm)