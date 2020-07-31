//
// Created by ccone on 6/26/2020.
//

#include <iostream>
#include "tictacAI.h"

tictacAI::tictacAI() {
    _is_generated = false;

    _tree_start = nullptr;
    _cur_game = nullptr;

    _ai_current_board_state = nullptr;
    _ai_turn = false;
    _ai_mark = EMPTY;
}

tictacAI::~tictacAI() {
    if(_is_generated) {
        delete _tree_start;
    }
    _tree_start = nullptr;
}

void tictacAI::generateTree() {
    if(!_is_generated) {
        board blank_board;
        _tree_start = new move_node(blank_board);

        int open_spots[BOARD_SIZE];
        for(int i = 0; i < BOARD_SIZE; ++i) {
            open_spots[i] = i;
        }

        rec_create_tree(_tree_start, true);
        _is_generated = true;
    }
}

int tictacAI::rec_create_tree(move_node *c_node, bool is_x_turn) {
    // check if its a winning move;
    if( c_node->isWinningPlay() ) {
        boardSpot mark = c_node->getWinningMark();
        c_node->setWinCounts(
                1*(mark == X_MARK),
                1*(mark == O_MARK)
                );
        return 0;
    }

    else if ( c_node->getBoard()->getOpenSpots() == 0) return 0;

    boardSpot move_mark = (is_x_turn ? X_MARK : O_MARK);
    int branched_x_wins = 0;
    int branched_o_wins = 0;

    for( int i = 0; i < BOARD_SIZE; i++ ) {
        if(c_node->getBoard()->checkPlace(i) == EMPTY) {
            board n_board = *c_node->getBoard();
            n_board.setPlace(i, move_mark);

            auto* n_node = new move_node(n_board);
            c_node->setBranch(i, n_node);

            rec_create_tree(n_node, !is_x_turn);

            branched_x_wins += n_node->getXWinCount();
            branched_o_wins += n_node->getOWinCount();
        }
    }
    c_node->setWinCounts(branched_x_wins, branched_o_wins);

    return 0;
}

int tictacAI::getXCount() {
    return _tree_start->getXWinCount();
}

int tictacAI::getOCount() {
    return _tree_start->getOWinCount();
}

int tictacAI::countNodes() {
    return _tree_start->countSubNodes();
}

int tictacAI::countNodesAtLevel(int level) {
    return rec_count_nodes_at_level(--level, _tree_start);
}

int tictacAI::rec_count_nodes_at_level(int level, tictacAI::move_node *node) {
    if(node == nullptr) {
        return 0;
    }

    else if(level != 0) {
        int count = 0;
        for (int i = 0; i < BOARD_SIZE; ++i) {
            count += rec_count_nodes_at_level(level-1, node->getBranch(i));
        }
        return count;
    }

    else {
        return 1;
    }
}

void tictacAI::startGame(boardSpot mark, gameBoard *game) {
    _ai_mark = mark;
    _cur_game = game;

    if(_ai_mark == X_MARK) {
        _ai_current_board_state = _tree_start;
        _ai_turn = true;
    } else {
        _ai_current_board_state = nullptr;
        _ai_turn = false;
    }
}

int tictacAI::AIGetNextMove() {
    if(!_ai_turn) return 0;

    int best_move_index = 0;
    float best_ratio = 0.0;
    bool can_win = false;

    for (int i = 0; i < BOARD_SIZE; ++i) {
        move_node* cur_option = _ai_current_board_state->getBranch(i);
        float ratio = 0.0;
        if(cur_option != nullptr) {
            // if the game can be won, win it
            if(cur_option->getWinningMark() == _ai_mark) {
                return i;

            } else {
                // check the win / loss ratio
                // TODO move into be part of the move node when adding pruning
                switch(_ai_mark) {
                    case X_MARK:
                        ratio = (float)cur_option->getXWinCount() / (float)cur_option->getOWinCount();
                        break;
                    case O_MARK:
                        ratio = (float)cur_option->getOWinCount() / (float)cur_option->getXWinCount();
                        break;
                    default:
                        ratio = 0;
                        break;
                }

                if(ratio > best_ratio) {
                    best_ratio = ratio;
                    best_move_index = i;
                }
            }
        }
    }
    _ai_turn = false;
    _ai_current_board_state = _ai_current_board_state->getBranch(best_move_index);

    return best_move_index;
}

void tictacAI::AISetPrevMove() {
    if(_ai_turn) return;
    _ai_current_board_state = _ai_current_board_state->getBranch(_cur_game->getIndexOfLastPlay());
    _ai_turn = true;
}

tictacAI::move_node::move_node() {
    _winning_mark = EMPTY;
    possible_x_wins = 0;
    possible_o_wins = 0;

    for (auto &i : _counter_moves) {
        i = nullptr;
    }
}

tictacAI::move_node::~move_node() {
    for (auto &i : _counter_moves) {
        delete i;
    }
}

tictacAI::move_node::move_node(const board& g_state) {
    cur_state = g_state;
    _winning_mark = EMPTY;
    possible_x_wins = 0;
    possible_o_wins = 0;

    for (auto &i : _counter_moves) {
        i = nullptr;
    }

    // TODO make this a part of the board class
    if(cur_state.getOpenSpots() < MIN_TURNS_TO_WIN) {
        _winning_mark = cur_state.checkWin();
    }
}

void tictacAI::move_node::setBranch(int i, tictacAI::move_node *branch) {
    _counter_moves[i] = branch;
}

bool tictacAI::move_node::isWinningPlay() {
    return _winning_mark != EMPTY;
}

int tictacAI::move_node::countSubNodes() {
    return recCountNodes(this);
}

int tictacAI::move_node::recCountNodes(tictacAI::move_node *node) {
    int sub_node_count = 0;
    for (int i = 0; i < BOARD_SIZE*(node != nullptr); ++i) {
        sub_node_count += recCountNodes(node->getBranch(i));
    }
    return sub_node_count + (node != nullptr);
}

void tictacAI::move_node::setWinCounts(int x_wins, int o_wins) {
    possible_o_wins = o_wins;
    possible_x_wins = x_wins;
}

