cmake_minimum_required(VERSION 3.1)

add_subdirectory(src)
add_subdirectory(test)
add_executable(chip8 main.cpp Chip8.cpp)
