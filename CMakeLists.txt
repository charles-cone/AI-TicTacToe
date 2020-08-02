# cmake_minimum_required(VERSION <specify CMake version here>)
project(tictactoes)

set(CMAKE_CXX_STANDARD 14)

add_executable(tictactoes main.cpp board/board.cpp board/board.h tictacAI.cpp tictacAI.h tests/test_functions.h tests/test_functions.cpp board/gameBoard.cpp board/gameBoard.h)