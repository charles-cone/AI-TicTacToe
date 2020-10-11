//
// Created by ccone on 9/21/2020.
//

#ifndef TICTACTOES_OPPONENTAI_H
#define TICTACTOES_OPPONENTAI_H

#include "../board/board.h"
#include "../board/gameBoard.h"

class opponentAI {
public:
    virtual ~opponentAI() = default;

    virtual void generateTree() = 0;
    virtual void startGame(gameBoard *game) = 0;

    virtual void aiTakeNextTurn() = 0;

    virtual int countAllNodes() = 0;
};


#endif //TICTACTOES_OPPONENTAI_H
