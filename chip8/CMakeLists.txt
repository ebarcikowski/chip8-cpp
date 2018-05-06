cmake_minimum_required(VERSION 3.1)

add_executable(chip8 main.cpp Chip8.cpp Chip8SDL.cpp)
target_link_libraries(chip8 SDL2 SDL2_mixer)
# Hacky way to add sound file
add_definitions(-DBONK_FNAME="${CMAKE_SOURCE_DIR}/res/media/bonk.wav")  
