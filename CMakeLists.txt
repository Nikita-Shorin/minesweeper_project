cmake_minimum_required(VERSION 3.21)
project(minesweeper)

set(CMAKE_CXX_STANDARD 20)

add_executable(minesweeper main.cpp minesweeper.cpp)
add_subdirectory(tests)
