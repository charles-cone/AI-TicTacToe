//
// Created by ccone on 9/21/2020.
//

#ifndef TICTACTOES_PRUNEDAI_H
#define TICTACTOES_PRUNEDAI_H

#include <limits>

#include "opponentAI.h"

class prunedAI : public opponentAI {
public:
    prunedAI();
    ~prunedAI() override;

    void generateTree() override;
    void startGame(gameBoard *game) override;

    void aiTakeNextTurn() override;

    int countAllNodes() override;

    // debug
    void debug();

private:
    class move_node {
    public:
        explicit move_node(move_node **counters, short len, short ai_i);
        ~move_node();

        move_node* getBranch(int i) { return _counter_moves[i]; }
        char getAiIndex() const { return _ai_play_index; }

        bool hasNoCounters() { return _counter_moves == nullptr; }
        void investigateNode();

    private:
        short _ai_play_index;
        short _counter_len;
        move_node** _counter_moves;
    };

    class node_tree {
    public:
        node_tree();
        ~node_tree();

        void generateTree();
        int countNodes();

        move_node* getHead() { return _head; }

        // debug
        [[noreturn]] void navigateTree();

    private:
        int _rec_count_children(int level, move_node *node);

        move_node* _rec_generate_game_node(board *board, int &ai_wins, int &h_wins);
        move_node** _rec_generate_human_turns(board *board, int &ai_wins, int &h_wins);

        move_node* _head;
        boardSpot _ai_mark;
        boardSpot _human_mark;
    };

    node_tree* _tree_start;
    move_node* _game_state;

    gameBoard* _board;
    boardSpot _ai_mark;
};


#endif //TICTACTOES_PRUNEDAI_H
