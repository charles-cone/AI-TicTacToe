# cmake_minimum_required(VERSION <specify CMake version here>)
project(tictactoes)

set(CMAKE_CXX_STANDARD 14)

add_executable(tictactoes src/main.cpp src/board/board.cpp src/board/board.h src/tictacAI.cpp src/tictacAI.h src/tests/test_functions.h src/tests/test_functions.cpp src/board/gameBoard.cpp src/board/gameBoard.h)
