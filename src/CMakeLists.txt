cmake_minimum_required(VERSION 3.1)

add_executable(chip8 main.cpp Chip8.cpp Chip8SDL.cpp)
target_link_libraries(chip8 SDL2)
