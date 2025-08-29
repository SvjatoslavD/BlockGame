cmake_minimum_required(VERSION 3.30)
project(BlockGame VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(src)

if (CMAKE_BUILD_TYPE STREQUAL "AllTests")
    enable_testing()
    add_subdirectory(tests)
endif ()