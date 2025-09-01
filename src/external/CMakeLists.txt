cmake_minimum_required(VERSION 3.30)

find_package(SFML COMPONENTS Network Graphics Window Audio System CONFIG REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm CONFIG REQUIRED)

# Create ImGui library
set(IMGUI_SOURCES
        ImGui/imgui.cpp
        ImGui/imgui_demo.cpp
        ImGui/imgui_draw.cpp
        ImGui/imgui_tables.cpp
        ImGui/imgui_widgets.cpp
        ImGui/backends/imgui_impl_opengl3.cpp
)

set(IMGUI_HEADERS
        ImGui/imconfig.h
        ImGui/imgui.h
        ImGui/imgui_internal.h
        ImGui/imstb_rectpack.h
        ImGui/imstb_textedit.h
        ImGui/imstb_truetype.h
        ImGui/backends/imgui_impl_opengl3.h
)

add_library(imgui STATIC ${IMGUI_SOURCES} ${IMGUI_HEADERS})
target_include_directories(imgui PUBLIC ImGui ImGui/backends)
target_link_libraries(imgui PUBLIC SFML::Graphics)

# Create ImGui-SFML library
set(IMGUI_SFML_SOURCES
        ImGui/imgui-SFML.cpp
)

set(IMGUI_SFML_HEADERS
        ImGui/imgui-SFML.h
)

add_library(imgui-sfml STATIC ${IMGUI_SFML_SOURCES} ${IMGUI_SFML_HEADERS})
target_include_directories(imgui-sfml PUBLIC ImGui)
target_link_libraries(imgui-sfml PUBLIC
    imgui
)

# Create Fast Noise Library
add_library(fast-noise-lite INTERFACE)
target_include_directories(fast-noise-lite INTERFACE FastNoiseLite)

# Bundle all external libraries
add_library(external-libs INTERFACE)
target_link_libraries(external-libs INTERFACE
        GLEW::GLEW
        SFML::Network SFML::Window SFML::Audio SFML::System
        glm::glm
        imgui-sfml
        fast-noise-lite
)
