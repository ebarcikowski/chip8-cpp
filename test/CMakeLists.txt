cmake_minimum_required(VERSION 3.1)

set(GTEST_DIR "../external/googletest")
include_directories(
  ${GTEST_DIR}/googletest
  ${GTEST_DIR}/googlemock
  )
add_executable(chip8test Chip8Tests.cpp)

target_link_libraries(chip8test gtest_main)
