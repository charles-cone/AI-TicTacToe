# cmake_minimum_required(VERSION <specify CMake version here>)
project(tictactoes)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(lib/easy_timer)

add_executable(tictactoes src/main.cpp src/board/board.cpp src/board/board.h src/ai/dynamicCounterAI.cpp src/ai/dynamicCounterAI.h src/tests/test_functions.h src/tests/test_functions.cpp src/board/gameBoard.cpp src/board/gameBoard.h src/ai/opponentAI.h src/ai/minMaxAI.cpp src/ai/minMaxAI.h)
target_link_libraries(tictactoes LINK_PUBLIC easy_timer)