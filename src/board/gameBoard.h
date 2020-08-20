//
// Created by ccone on 7/28/2020.
//

#ifndef TICTACTOES_GAMEBOARD_H
#define TICTACTOES_GAMEBOARD_H

#include <iostream>

#include "board.h"

class gameBoard : public board {
public:
    gameBoard() = default;
    ~gameBoard() = default;

    void printBoard();
    void promptUserInput(boardSpot mark);
    void playByOpenIndex(int o_i, boardSpot mark);

    int getIndexOfLastPlay();


private:
    int _last_play_index;
    boardSpot _last_play_mark;

};


#endif //TICTACTOES_GAMEBOARD_H
