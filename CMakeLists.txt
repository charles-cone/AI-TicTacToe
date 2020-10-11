cmake_minimum_required(VERSION 3.17)
project(tictactoes)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(lib/easy_timer)

add_executable(tictactoes
        src/main.cpp src/board/board.cpp src/board/board.h
        src/ai/dynamicCounterAI.cpp src/ai/dynamicCounterAI.h
        src/tests/test_functions.h src/tests/test_functions.cpp
        src/board/gameBoard.cpp src/board/gameBoard.h
        src/ai/prunedAI.cpp src/ai/prunedAI.h
        src/ai/opponentAI.h )

target_link_libraries(tictactoes LINK_PUBLIC easy_timer)