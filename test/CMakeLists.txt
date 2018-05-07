cmake_minimum_required(VERSION 3.1)

set(GTEST_DIR "../external/googletest")
include_directories(
  ${GTEST_DIR}/googletest
  ${GTEST_DIR}/googlemock
  ${CMAKE_SOURCE_DIR}/chip8
  )

add_executable(chip8test
  Chip8Tests.cpp
  Chip8Mock.cpp
  Chip8OpTests.cpp
  ${CMAKE_SOURCE_DIR}/chip8/Chip8.cpp)

target_link_libraries(chip8test gmock gtest_main)
