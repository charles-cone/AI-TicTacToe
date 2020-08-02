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
        explicit move_node(board *g_state);
        ~move_node();

        int getXWinCount() { return _possible_x_wins; }
        int getOWinCount() { return _possible_o_wins; }
        int countSubNodes();

        float getXWinRatio() { return _ratio; }
        float getOWinRatio() { return 1 / _ratio; }

        void setWinCounts(int x_wins, int o_wins);
        void setBranch(int i, move_node* branch);

        bool isWinningPlay();
        boardSpot getWinningMark() { return _winning_mark; }

        move_node* getBranch(int i) { return _counter_moves[i]; }


    private:
        static int rec_count_nodes(move_node* node);

        boardSpot _winning_mark;

        int _possible_x_wins;
        int _possible_o_wins;

        float _ratio;

        move_node* _counter_moves[BOARD_SIZE];
    };

    int rec_create_tree(move_node *c_node, bool is_x_turn, board *c_board);
    int rec_count_nodes_at_level(int level, move_node* node);

    bool _is_generated;
    bool _ai_turn;

    boardSpot _ai_mark;

    move_node* _tree_start;
    move_node* _ai_current_board_state;

    gameBoard* _cur_game;
};


#endif //TICTACTOES_TICTACAI_H
