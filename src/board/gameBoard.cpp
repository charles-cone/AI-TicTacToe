//
// Created by ccone on 7/28/2020.
//

#include "gameBoard.h"

/**
 * Prompt the user on the console for their input
 * @param mark The players mark
 */
void gameBoard::promptUserInput(boardSpot mark) {
    bool invalid_input = true;
    std::cout << "- Enter your move -" << std::endl;
    while(invalid_input) {
        int r = 0;
        int c = 0;
        std::cout << "Row: ";
        std::cin >> r;
        std::cout << "Column: ";
        std::cin >> c;

        if( setPlace(r, c, mark) ) {
            _last_play_index = 3*r + c;
            invalid_input = false;
        } else {
            std::cout << "Spot is already taken, chose another" << std::endl;
        }
    }
}

/**
 * Print the board in ascii art to console
 */
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

/**
 * gets the index of the last move made
 * @return Index of the last move
 */
int gameBoard::getIndexOfLastPlay() {
    return _last_play_index;
}

/**
 * Play a mark, indexed by only the open spaces
 * @param o_i The open index to play at
 * @param mark The mark to place
 */
void gameBoard::playByOpenIndex(int o_i, boardSpot mark) {
    for(int i = 0; i < BOARD_SIZE; i++) {
        if (checkPlace(i) == EMPTY) {
            if (o_i == 0) {
                setPlace(i, mark);
                return;
            } else {
                o_i--;
            }
        }
    }
}

/**
 * Erases all marks on board
 */
void gameBoard::resetBoard() {
    for(int i = 0; i < BOARD_SIZE; i++) {
        erasePlay(i);
    }
}
