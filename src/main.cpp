#include <iostream>

#include "ai/prunedAI.h"
#include "board/gameBoard.h"


int main() {
    auto* gameAI = new prunedAI();
    auto* cBoard = new gameBoard();

    gameAI->generateTree();
    bool quit = false;


    while(!quit) {
        cBoard->resetBoard();
        gameAI->startGame(cBoard);

        bool isAITurn = true; // X goes first
        while (cBoard->canPlay()) {
            if(isAITurn){
                std::cout << " - AI Move - " << std::endl;
                gameAI->aiTakeNextTurn();
            } else {
                cBoard->promptUserInput(O_MARK);
            }

            isAITurn = !isAITurn;
            cBoard->printBoard();
        }

        switch (cBoard->checkWin()) {
            case X_MARK:
                std::cout << "The AI Wins!!";
                break;
            case O_MARK:
                std::cout << "You Win!!";
                break;
            case EMPTY:
                std::cout << "It's A Draw!!";
                break;
        }

        std::cout << " Keep Playing? (q to stop, anything else to continue)" << std::endl;
        char input;
        std::cin >> input;
        quit = input == 'q' || input == 'Q';
    }

    delete cBoard;
    delete gameAI;
    return 0;
}