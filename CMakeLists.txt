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
        core/opengl/Shader.cpp
        core/opengl/EBO.cpp
        core/opengl/VAO.cpp
        core/opengl/VBO.cpp
        core/opengl/Texture.cpp
        core/Chunk.cpp
        core/Camera.cpp
        core/World.cpp
        core/opengl/Shader.h
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
        core/application/Renderer.cpp
        core/application/Renderer.h
        core/states/TitleState.cpp
        core/states/TitleState.h
)

# Create ImGui library
set(IMGUI_SOURCES
        external/ImGui/imgui.cpp
        external/ImGui/imgui_demo.cpp
        external/ImGui/imgui_draw.cpp
        external/ImGui/imgui_tables.cpp
        external/ImGui/imgui_widgets.cpp
        external/ImGui/backends/imgui_impl_opengl3.cpp
)

set(IMGUI_HEADERS
        external/ImGui/imconfig.h
        external/ImGui/imgui.h
        external/ImGui/imgui_internal.h
        external/ImGui/imstb_rectpack.h
        external/ImGui/imstb_textedit.h
        external/ImGui/imstb_truetype.h
        external/ImGui/backends/imgui_impl_opengl3.h
)

add_library(imgui STATIC ${IMGUI_SOURCES} ${IMGUI_HEADERS})
target_include_directories(imgui PUBLIC external/ImGui/ external/ImGui/backends)
target_link_libraries(imgui PUBLIC SFML::Graphics)

# Create ImGui-SFML library
set(IMGUI_SFML_SOURCES
        external/ImGui/imgui-SFML.cpp
)

set(IMGUI_SFML_HEADERS
        external/ImGui/imgui-SFML.h
)

add_library(imgui-sfml STATIC ${IMGUI_SFML_SOURCES} ${IMGUI_SFML_HEADERS})
target_include_directories(imgui-sfml PUBLIC external/ImGui/imgui-sfml)
target_link_libraries(imgui-sfml PUBLIC imgui)

set(LIBS
        GLEW::GLEW
        SFML::Network SFML::Graphics SFML::Window SFML::Audio SFML::System
        glm::glm
        imgui-sfml
)

add_executable(MyGame main.cpp ${SOURCE})

target_link_libraries(MyGame PRIVATE ${LIBS})

enable_testing()

find_package(GTest CONFIG REQUIRED)

set(TESTS
        tests/TestApplication.cpp
        tests/ApplicationFixture.cpp
        tests/ApplicationFixture.h
)

add_executable(MyTests ${TESTS} ${SOURCE})

target_link_libraries(MyTests PRIVATE ${LIBS} GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)

add_test(NAME AllTests COMMAND MyTests)