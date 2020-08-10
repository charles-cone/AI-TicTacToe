# cmake_minimum_required(VERSION <specify CMake version here>)
project(tictactoes)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(lib/easy_timer)
#find_library(EASY_TIMER easy_timer lib/easy_timer)
include_directories(lib/easy_timer)

add_executable(tictactoes src/main.cpp src/board/board.cpp src/board/board.h src/tictacAI.cpp src/tictacAI.h src/tests/test_functions.h src/tests/test_functions.cpp src/board/gameBoard.cpp src/board/gameBoard.h)
target_link_libraries(tictactoes LINK_PUBLIC ${EASY_TIMER})