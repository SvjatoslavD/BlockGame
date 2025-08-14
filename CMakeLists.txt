cmake_minimum_required(VERSION 3.30)
project(BlockGame)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_SOURCE_DIR ${CMAKE_BINARY_DIR})

find_package(SFML COMPONENTS Network Graphics Window Audio System CONFIG REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm CONFIG REQUIRED)

set(LIBS
        GLEW::GLEW
        SFML::Network SFML::Graphics SFML::Window SFML::Audio SFML::System
        glm::glm
)

set(SOURCE
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
        core/application/StateManager.cpp
        core/application/StateManager.h
        core/states/GameState.h
        core/Tilemap.cpp
        core/Tilemap.h
        core/application/Application.cpp
        core/application/Application.h
)

add_executable(MyGame main.cpp ${SOURCE})

target_link_libraries(MyGame PRIVATE ${LIBS})

enable_testing()

find_package(GTest CONFIG REQUIRED)

set(TESTS
        tests/TestApplication.cpp
)

add_executable(MyTests ${TESTS} ${SOURCE})

target_link_libraries(MyTests PRIVATE ${LIBS} GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)

add_test(NAME AllTests COMMAND MyTests)