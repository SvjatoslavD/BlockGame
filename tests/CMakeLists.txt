cmake_minimum_required(VERSION 3.30)

# Find GTest
find_package(GTest REQUIRED)

# Collect test files
file(GLOB_RECURSE TEST_SOURCES
        "*.cpp"
        "*.h"
)

# Create test executable
add_executable(AllTests ${TEST_SOURCES})

# Link libraries
target_link_libraries(AllTests PRIVATE
        GTest::GTest
        GTest::Main
        external_libs
)

# Include directories
target_include_directories(AllTests PRIVATE
        ${CMAKE_SOURCE_DIR}/src  # Access to main app headers
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# Register tests with CTest
add_test(NAME GameTests COMMAND AllTests)