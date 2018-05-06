cmake_minimum_required(VERSION 3.1)
project(chip8-cpp)

set(CMAKE_CXX_STANDARD 17)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

add_subdirectory(chip8)
add_subdirectory(external/googletest)
add_subdirectory(test)
