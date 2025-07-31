cmake_minimum_required(VERSION 3.30)
project(BlockGame)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_SOURCE_DIR ${CMAKE_BINARY_DIR})

find_package(SFML COMPONENTS Network Graphics Window Audio System CONFIG REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm CONFIG REQUIRED)

set(SOURCE_FILES
        main.cpp
        src/Shader.cpp
        src/windowSetup.cpp
        src/EBO.cpp
        src/VAO.cpp
        src/VBO.cpp
        src/Texture.cpp
        src/Chunk.cpp
        src/Camera.cpp
)

set(HEADER_FILES
        header/Shader.h
        header/windowSetup.h
        header/EBO.h
        header/VAO.h
        header/VBO.h
        header/gameVariables.h
        header/Texture.h
        header/Chunk.h
        header/Camera.h
        external/FastNoiseLite.h
)

add_executable(main ${HEADER_FILES} ${SOURCE_FILES} )

target_link_libraries(main PRIVATE
        GLEW::GLEW
        SFML::Network SFML::Graphics SFML::Window SFML::Audio SFML::System
        glm::glm)