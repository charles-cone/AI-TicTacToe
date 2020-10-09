//
// Created by ccone on 6/26/2020.
//

#ifndef TICTACTOES_DYNAMICCOUNTERAI_H
#define TICTACTOES_DYNAMICCOUNTERAI_H

#include <type_traits>
#include "../board/board.h"
#include "../board/gameBoard.h"
#include "opponentAI.h"

class dynamicCounterAI : public opponentAI {
public:
    dynamicCounterAI();
    ~dynamicCounterAI() override;

    void generateTree(boardSpot ai) override;
    void startGame(gameBoard *game) override;

    void aiTakeNextTurn() override;

    int countChildren() override;
    long int getTreeSize() override;

    int getXCount();
    int getOCount();

    int countNodesAtLevel(int level);

    void printWinGrid();

private:
    class move_node {
    public:
        move_node();
        explicit move_node(board *g_state);
        ~move_node();

        int getXWinCount() const { return _possible_x_wins; }
        int getOWinCount() const { return _possible_o_wins; }
        int countSubNodes();

        float getXWinRatio() const { return _ratio; }
        float getOWinRatio() const { return 1 / _ratio; }

        void setWinCounts(int x_wins, int o_wins);
        void setBranch(int i, move_node* branch);
        void inspect();

        bool isWinningPlay();
        boardSpot getWinningMark() { return _winning_mark; }

        move_node* getBranch(int i) { return _counter_moves[i]; }


    private:
        static int rec_count_nodes(move_node* node);

        boardSpot _winning_mark;

        int _possible_x_wins;
        int _possible_o_wins;

        float _ratio;

        int _arr_size;
        move_node **_counter_moves;
    };

    int rec_create_tree(move_node *c_node, bool is_x_turn, board *c_board);
    int rec_count_nodes_at_level(int level, move_node* node);

    // not the best, but lazy and fast way to refactor
    void ai_set_prev_move();
    int ai_get_next_move();

    bool _is_generated;
    bool _ai_turn;

    boardSpot _ai_mark;

    move_node* _tree_start;
    move_node* _ai_current_board_state;

    gameBoard* _cur_game;
};


#endif //TICTACTOES_DYNAMICCOUNTERAI_H
