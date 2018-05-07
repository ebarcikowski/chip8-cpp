cmake_minimum_required(VERSION 3.1)
project(chip8-cpp)

option(BUILD_WITH_TESTS OFF)


set(CMAKE_CXX_STANDARD 11)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

add_subdirectory(chip8)
if (BUILD_WITH_TESTS)
  set(CMAKE_CXX_STANDARD 17)
  add_subdirectory(external/googletest)
  add_subdirectory(test)
endif()
