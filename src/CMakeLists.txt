cmake_minimum_required(VERSION 3.30)

# Collect all source files
file(GLOB_RECURSE SOURCES
        "*.cpp"
        "*.h"
)

# Create the main executable
add_executable(${PROJECT_NAME} ${SOURCES} ../main.cpp)

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/application
        ${CMAKE_CURRENT_SOURCE_DIR}/opengl
        ${CMAKE_CURRENT_SOURCE_DIR}/states
)

# Link external libraries
target_link_libraries(${PROJECT_NAME} PRIVATE external-libs)