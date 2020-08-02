//
// Created by ccone on 7/24/2020.
//

#include <iostream>

#include "test_functions.h"
#include "../tictacAI.h"
#include "../board/board.h"
#include "../board/gameBoard.h"

void testWinValidation() {
    board row_b;
    board col_b;
    board diag_r_b;
    board diag_l_b;

    for (int i = 0; i < BOARD_SIDE_LENGTH; ++i) {
        row_b.setPlace(i, 0, X_MARK);
        col_b.setPlace(0, i, X_MARK);
        diag_r_b.setPlace(i, i, X_MARK);
        diag_l_b.setPlace(BOARD_SIDE_LENGTH - (i+1), BOARD_SIDE_LENGTH - (i+1), X_MARK);
    }


    std::cout << "TESTING: row board: ";
    if(row_b.checkWin() == X_MARK) {
        std::cout << "SUCCESS" << std::endl;
    }

    std::cout << "TESTING: col board: ";
    if(col_b.checkWin() == X_MARK) {
        std::cout << "SUCCESS" << std::endl;
    }

    std::cout << "TESTING: r_diag board: ";
    if(diag_r_b.checkWin() == X_MARK) {
        std::cout << "SUCCESS" << std::endl;
    }

    std::cout << "TESTING: l_diag board: ";
    if(diag_l_b.checkWin() == X_MARK) {
        std::cout << "SUCCESS" << std::endl;
    }

}

void testTreeBuild() {
    tictacAI gameTree;
    gameTree.generateTree();

    std::cout << "There is " << gameTree.getXCount() << " ways for X to win" << std::endl;
    std::cout << "There is " << gameTree.getOCount() << " ways for O to win" << std::endl;
    std::cout << "Tree has " << gameTree.countNodes() << " total nodes" << std::endl;

    int tot_nodes = 0;
    for (int i = 0; i < 12; ++i) {
        int nodes_at_level = gameTree.countNodesAtLevel(i);
        std::cout << "On level " << i << " there is " << nodes_at_level << " node(s)" << std::endl;
        tot_nodes += nodes_at_level;
    }
    std::cout << "counted " << tot_nodes << "   nodes total" << std::endl;
}

void testGameBoard() {
    gameBoard game_board;
    boardSpot winType;

    int i = 0;
    while (i < 5) {
        int row_choice;
        int col_choice;

        std::cout << "row:";
        std::cin >> row_choice;
        std::cout << "col:";
        std::cin >> col_choice;

        if(game_board.setPlace(row_choice, col_choice, X_MARK)) {
            game_board.printBoard();
            std::cout << std::endl;
            i++;
        } else {
            std::cout << "Space is already occupied" << std::endl;
        }
    }

    switch(game_board.checkWin()) {
        case EMPTY:
            std::cout << "It's a Draw!" << std::endl;
            break;
        case X_MARK:
            std::cout << "X Wins!" << std::endl;
            break;
        case O_MARK:
            std::cout << "O Wins!" << std::endl;
            break;
    }
}

void testAIMoves() {
    tictacAI game_tree;
    boardSpot ai_mark;
    boardSpot player_mark;
    gameBoard game_board;

    game_tree.generateTree();

    ai_mark = X_MARK;
    player_mark = O_MARK;

    game_tree.startGame(ai_mark, &game_board);

    while( game_board.canPlay() ) {
        int ai_move = game_tree.AIGetNextMove();
        game_board.setPlace(ai_move, ai_mark);
        game_board.printBoard();
        game_board.promptUserInput(player_mark);
        game_board.printBoard();
        game_tree.AISetPrevMove();
    }

}

void testObjects() {
    board game_board;
    game_board.setPlace(4, X_MARK);
}