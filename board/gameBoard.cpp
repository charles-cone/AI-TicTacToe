//
// Created by ccone on 7/28/2020.
//

#include "gameBoard.h"

void gameBoard::promptUserInput(boardSpot mark) {
    bool invalid_input = true;
    std::cout << "- Enter your move -" << std::endl;
    while(invalid_input) {
        int r, c;
        std::cout << "Row: ";
        std::cin >> r;
        std::cout << "Column: ";
        std::cin >> c;

        if( setPlace(r, c, mark) ) {
            _last_play_index = 3*r + c;
            _last_play_mark = mark;
            invalid_input = false;
        } else {
            std::cout << "Spot is already taken, chose another" << std::endl;
        }
    }
}

void gameBoard::printBoard() {
    for(int i = 0; i < BOARD_SIDE_LENGTH; ++i) {
        for (int j = 0; j < BOARD_SIDE_LENGTH; ++j) {
            char sign;

            if(checkPlace(i, j) == EMPTY) sign = ' ';
            else if(checkPlace(i, j) == X_MARK) sign = 'X';
            else sign = 'O';

            std::cout << sign << " ";

            if(j != BOARD_SIDE_LENGTH - 1) {
                std::cout << "| ";
            }
        }

        if(i != BOARD_SIDE_LENGTH - 1) {
            std::cout << std::endl << "--+---+--" << std::endl;
        }
    }
    std::cout << std::endl;
}

int gameBoard::getIndexOfLastPlay() {
    return _last_play_index;
}
