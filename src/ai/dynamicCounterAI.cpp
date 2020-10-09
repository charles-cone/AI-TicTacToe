//
// Created by ccone on 6/26/2020.
//

#include <iostream>
#include "dynamicCounterAI.h"

dynamicCounterAI::dynamicCounterAI() {
    _is_generated = false;

    _tree_start = nullptr;
    _cur_game = nullptr;

    _ai_current_board_state = nullptr;
    _ai_turn = false;
    _ai_mark = EMPTY;
}

dynamicCounterAI::~dynamicCounterAI() {
    if(_is_generated) {
        delete _tree_start;
    }
    _tree_start = nullptr;
}

void dynamicCounterAI::generateTree(boardSpot ai) {
    if(!_is_generated) {
        _ai_mark = ai;
        auto* blank_board = new board();
        _tree_start = new move_node(blank_board);

        rec_create_tree(_tree_start, true, blank_board);
        _is_generated = true;
    }
}

int dynamicCounterAI::rec_create_tree(move_node *c_node, bool is_x_turn, board *c_board) {
    // check if its a winning move;
    if( c_node->isWinningPlay() ) {
        boardSpot mark = c_node->getWinningMark();
        c_node->setWinCounts(
                1*(mark == X_MARK),
                1*(mark == O_MARK)
                );
        return 0;
    }

    else if ( c_board->getOpenSpots() == 0) return 0;

    boardSpot move_mark = (is_x_turn ? X_MARK : O_MARK);
    int branched_x_wins = 0;
    int branched_o_wins = 0;
    int placed_marks = 0;

    for( int i = 0; i < BOARD_SIZE; i++ ) {
        if(c_board->checkPlace(i) == EMPTY) {

            c_board->setPlace(i, move_mark);

            auto* n_node = new move_node(c_board);
            c_node->setBranch(placed_marks, n_node);

            rec_create_tree(n_node, !is_x_turn, c_board);

            c_board->erasePlay(i);

            branched_x_wins += n_node->getXWinCount();
            branched_o_wins += n_node->getOWinCount();
            placed_marks++;
        }
    }
    c_node->setWinCounts(branched_x_wins, branched_o_wins);

    return 0;
}

int dynamicCounterAI::getXCount() {
    return _tree_start->getXWinCount();
}

int dynamicCounterAI::getOCount() {
    return _tree_start->getOWinCount();
}

int dynamicCounterAI::countChildren() {
    return _tree_start->countSubNodes();
}

int dynamicCounterAI::countNodesAtLevel(int level) {
    return rec_count_nodes_at_level(--level, _tree_start);
}

int dynamicCounterAI::rec_count_nodes_at_level(int level, dynamicCounterAI::move_node *node) {
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

void dynamicCounterAI::startGame( gameBoard *game ) {
    _cur_game = game;

    if(_ai_mark == X_MARK) {
        _ai_current_board_state = _tree_start;
        _ai_turn = true;
    } else {
        _ai_current_board_state = nullptr;
        _ai_turn = false;
    }
}

int dynamicCounterAI::ai_get_next_move() {
    if(!_ai_turn) return 0;

    int best_move_index = 0;
    float best_ratio = 0;

    for (int i = 0; i < _cur_game->getOpenSpots(); ++i) {
        move_node* cur_option = _ai_current_board_state->getBranch(i);
        float ratio;
        if(cur_option != nullptr) {
            cur_option->inspect();
            // if the game can be won, win it
            if(cur_option->getWinningMark() == _ai_mark || _cur_game->getOpenSpots() == 1) {
                return i;

            } else {
                // check the win / loss ratio
                if(_ai_mark == X_MARK) {
                    ratio = cur_option->getXWinRatio();
                } else {
                    ratio = cur_option->getOWinRatio();
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

void dynamicCounterAI::ai_set_prev_move() {
    if(_ai_turn) return;
    // convert the board space to an index that can be used in the counter array
    int last_play_open_index = 0;
    for(int i = _cur_game->getIndexOfLastPlay(); i > 0; i--) {
        last_play_open_index += _cur_game->checkPlace(i) == EMPTY;
    }

    _ai_current_board_state = _ai_current_board_state->getBranch(last_play_open_index);
    _ai_turn = true;
}

long int dynamicCounterAI::getTreeSize() {
    return 0;
}

void dynamicCounterAI::aiTakeNextTurn() {
    ai_set_prev_move();
    _cur_game->playByOpenIndex(ai_get_next_move(), _ai_mark);
}

void dynamicCounterAI::printWinGrid() {
    for (int i = 0; i < 3; ++i) {
        int row = i*3;
        std::cout << _tree_start->getBranch(row)->getXWinCount() << "|"
                  << _tree_start->getBranch(row+1)->getXWinCount() << "|"
                  << _tree_start->getBranch(row+2)->getXWinCount() << std::endl;

        std::cout << _tree_start->getBranch(row)->getOWinCount() << "|"
                  << _tree_start->getBranch(row+1)->getOWinCount() << "|"
                  << _tree_start->getBranch(row+2)->getOWinCount() << std::endl;

        std::cout << std::endl;
    }
    std::cout << "totals X:" << _tree_start->getXWinCount() << " O:" << _tree_start->getOWinCount() << std::endl;
}

dynamicCounterAI::move_node::move_node() {
    _winning_mark = EMPTY;
    _possible_x_wins = 0;
    _possible_o_wins = 0;
    _ratio = 0.0;
    _arr_size = 0;

    for (int i = 0; i < _arr_size; i++) {
        _counter_moves[i] = nullptr;
    }
}

dynamicCounterAI::move_node::move_node(board *g_state) {
    _winning_mark = EMPTY;
    _possible_x_wins = 0;
    _possible_o_wins = 0;
    _ratio = 0.0;
    _arr_size = g_state->getOpenSpots();

    _counter_moves = new move_node*[_arr_size];

    for (int i = 0; i < _arr_size; i++) {
        _counter_moves[i] = nullptr;
    }

    if(g_state->getOpenSpots() < MIN_TURNS_TO_WIN) {
        _winning_mark = g_state->checkWin();
    }
}

dynamicCounterAI::move_node::~move_node() {
    for (int i = 0; i < _arr_size; i++) {
        delete _counter_moves[i];
    }
    delete [] _counter_moves;
}

void dynamicCounterAI::move_node::setBranch(int i, dynamicCounterAI::move_node *branch) {
    _counter_moves[i] = branch;
}

bool dynamicCounterAI::move_node::isWinningPlay() {
    return _winning_mark != EMPTY;
}

int dynamicCounterAI::move_node::countSubNodes() {
    return rec_count_nodes(this);
}

int dynamicCounterAI::move_node::rec_count_nodes(dynamicCounterAI::move_node *node) {
    int sub_node_count = 0;
    for (int i = 0; i < BOARD_SIZE*(node != nullptr); ++i) {
        sub_node_count += rec_count_nodes(node->getBranch(i));
    }
    return sub_node_count + (node != nullptr);
}

void dynamicCounterAI::move_node::setWinCounts(int x_wins, int o_wins) {
    _possible_o_wins = o_wins;
    _possible_x_wins = x_wins;
    _ratio = _ratio = (float)_possible_x_wins / (float)_possible_o_wins;
}

void dynamicCounterAI::move_node::inspect() {
    move_node* arr[9];
    for(int i = 0; i < 9; i++){
        if(i < _arr_size) arr[i] == _counter_moves[i];
        else arr[i] == nullptr;
    }
    return;
}
