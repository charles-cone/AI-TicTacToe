//
// Created by ccone on 6/26/2020.
//

#ifndef TICTACTOES_TICTACAI_H
#define TICTACTOES_TICTACAI_H

#include <type_traits>
#include "board/board.h"
#include "board/gameBoard.h"

class tictacAI {
public:
    tictacAI();
    ~tictacAI();

    void generateTree();
    void startGame(boardSpot mark, gameBoard *game);
    void AISetPrevMove();

    int getXCount();
    int getOCount();

    int countNodes();
    int countNodesAtLevel(int level);

    int AIGetNextMove();

private:
    class move_node {
    public:
        move_node();
        explicit move_node(const board& g_state);
        ~move_node();

        int getXWinCount() { return possible_x_wins; }
        int getOWinCount() { return possible_o_wins; }
        int countSubNodes();


        void setWinCounts(int x_wins, int o_wins);
        void setBranch(int i, move_node* branch);

        board* getBoard() { return &cur_state; }

        bool isWinningPlay();
        boardSpot getWinningMark() { return _winning_mark; }

        move_node* getBranch(int i) { return _counter_moves[i]; }


    private:
        static int recCountNodes(move_node* node);

        boardSpot _winning_mark;

        int possible_x_wins;
        int possible_o_wins;

        board cur_state;

        move_node* _counter_moves[BOARD_SIZE];
    };

    int rec_create_tree(move_node *c_node, bool is_x_turn);
    int rec_count_nodes_at_level(int level, move_node* node);

    bool _is_generated;
    bool _ai_turn;

    boardSpot _ai_mark;

    move_node* _tree_start;
    move_node* _ai_current_board_state;

    gameBoard* _cur_game;
};


#endif //TICTACTOES_TICTACAI_H
