cmake_minimum_required(VERSION 3.30)

add_subdirectory(external)

# Collect all source files
file(GLOB_RECURSE SOURCES
        "*.cpp"
        "*.h"
)

list(REMOVE_ITEM SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/external")

# Create the main executable
add_executable(${PROJECT_NAME} ${SOURCES} main.cpp)

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/core
        ${CMAKE_CURRENT_SOURCE_DIR}/entities
        ${CMAKE_CURRENT_SOURCE_DIR}/managers
        ${CMAKE_CURRENT_SOURCE_DIR}/opengl
        ${CMAKE_CURRENT_SOURCE_DIR}/rendering
        ${CMAKE_CURRENT_SOURCE_DIR}/states
        ${CMAKE_CURRENT_SOURCE_DIR}/world
)

# Link external libraries
target_link_libraries(${PROJECT_NAME} PRIVATE external-libs)