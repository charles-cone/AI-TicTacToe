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

/* generates the AI's game tree, sets the generated flag
 *
 * @param ai The mark that the AI will use (CURRENTLY ONLY WORKS WITH X)
 * @return nothing
 * */
void dynamicCounterAI::generateTree() {
    if(!_is_generated) {
        auto* blank_board = new board();
        _tree_start = new move_node(blank_board);

        rec_create_tree(_tree_start, true, blank_board);
        _is_generated = true;
    }
}

/* recursive function used to create each node and add it's counters
 *
 * @param c_node A pointer to the move_node to populate with ratio and counter moves
 * @param is_x_turn Boolean value to know which mark to place
 * @param c_board Pointer to the board representing the game
 * @return nothing
 * */
void dynamicCounterAI::rec_create_tree(move_node *c_node, bool is_x_turn, board *c_board) {
    if( c_node->isWinningPlay() ) {
        boardSpot mark = c_node->getWinningMark();
        c_node->setWinCounts(
                1*(mark == X_MARK),
                1*(mark == O_MARK)
                );
        return;
    }

    else if ( c_board->getOpenSpots() == 0) return;

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
}

/*  Used to make sure that the tree calculates the proper number of winning games
 *
 * @param None
 * @return The count of X (or O) winning games from the head of the tree
 * */
int dynamicCounterAI::getXCount() {
    return _tree_start->getXWinCount();
}

int dynamicCounterAI::getOCount() {
    return _tree_start->getOWinCount();
}

// Calls countSubNodes and recursively counts the number of nodes
// @param None
// @return The total number of move_nodes in the game tree

int dynamicCounterAI::countAllNodes() {
    return _tree_start->countSubNodes();
}


int dynamicCounterAI::countNodesAtLevel(int level) {
    return rec_count_nodes_at_level(--level, _tree_start);
}


/**
 * Recursive function to total the number of nodes at a specific level
 * @param level The level's nodes to count, starts at 0
 * @param node A node in the tree, starting from the head
 * @return The number of nodes on level
 */
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

/**
 * Starts a new game against the AI
 * @param game Pointer to the new gameBoard used by the human
 */
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

/**
 * Gets the AI's next move, ai_set_prev_move MUST be called before
 * @return the AI's move as an index on the board
 */
int dynamicCounterAI::ai_get_next_move() {
    if(!_ai_turn) return 0;

    int best_move_index = 0;
    float best_ratio = 0;
    for (int i = 0; i < _cur_game->getOpenSpots(); ++i) {
        move_node* cur_option = _ai_current_board_state->getBranch(i);
        float ratio;
        if(cur_option != nullptr) {
            cur_option->inspect();
            // if the move is winning or there is only one option take it
            if(cur_option->getWinningMark() == _ai_mark || _cur_game->getOpenSpots() == 1) {
                return i;

            } else {
                // check the win / loss ratio to find the best node to go to
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

/**
 * Adjusts game state based on human move MUST be called before ai_get_next_move
 */
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

/*
 * Have the AI make it's next turn on the board
 */
void dynamicCounterAI::aiTakeNextTurn() {
    ai_set_prev_move();
    _cur_game->playByOpenIndex(ai_get_next_move(), _ai_mark);
}

/**
 * Constructor for the move node, allocates the counter array and checks if board is winning
 * @param g_state Pointer to the board that the node is representing
 */
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

/**
 * Deallocates the counter array and ALL of it's sub-nodes
 */
dynamicCounterAI::move_node::~move_node() {
    for (int i = 0; i < _arr_size; i++) {
        delete _counter_moves[i];
    }
    delete [] _counter_moves;
}

/**
 * Set a node pointer in the counter array
 * @param i Index of counter array to set
 * @param branch Pointer to the new sub-node
 */
void dynamicCounterAI::move_node::setBranch(int i, dynamicCounterAI::move_node *branch) {
    _counter_moves[i] = branch;
}

/**
 * Check if the node represents a winning move
 * @return boolean if node is winning or not
 */
bool dynamicCounterAI::move_node::isWinningPlay() {
    return _winning_mark != EMPTY;
}

/**
 * recursively makes a count of all nodes in the tree
 * @return The total number of nodes in the tree
 */
int dynamicCounterAI::move_node::countSubNodes() {
    return rec_count_nodes(this);
}

/**
 * Recursively count all the sub-nodes under node
 * @param node The start node to count
 * @return the final count of sub-nodes
 */
int dynamicCounterAI::move_node::rec_count_nodes(dynamicCounterAI::move_node *node) {
    int sub_node_count = 0;
    for (int i = 0; i < BOARD_SIZE*(node != nullptr); ++i) {
        sub_node_count += rec_count_nodes(node->getBranch(i));
    }
    return sub_node_count + (node != nullptr);
}

/**
 * Set the combined values of all sub-node wins
 * @param x_wins count of all x_wins
 * @param o_wins
 */
void dynamicCounterAI::move_node::setWinCounts(int x_wins, int o_wins) {
    _possible_o_wins = o_wins;
    _possible_x_wins = x_wins;
    _ratio = _ratio = (float)_possible_x_wins / (float)_possible_o_wins;
}

/**
 * Used to aid in debugging, reads all counters into static-sized array
 */
void dynamicCounterAI::move_node::inspect() {
    move_node* arr[9];
    for(int i = 0; i < 9; i++){
        if(i < _arr_size) arr[i] == _counter_moves[i];
        else arr[i] == nullptr;
    }
    return; // place a breakpoint here during debugging
}
