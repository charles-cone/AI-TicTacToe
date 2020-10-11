//
// Created by ccone on 9/21/2020.
//

#include "prunedAI.h"

// PRUNE AI

prunedAI::prunedAI() {
    _tree_start = nullptr;
    _game_state = nullptr;
    _ai_mark = EMPTY;
}

prunedAI::~prunedAI() {
    delete _tree_start;
}

/**
 * allocates and creates the game tree
 */
void prunedAI::generateTree() {
    if(_tree_start == nullptr) {
        _ai_mark = X_MARK;
        _tree_start = new node_tree();
        _tree_start->generateTree();
    }
}

/**
 * Starts a new game against the AI
 * @param game pointer to the board that the game will be played on
 */
void prunedAI::startGame(gameBoard *game) {
    if(_tree_start != nullptr) {
        if(_ai_mark == X_MARK) {
            _game_state = _tree_start->getHead();
        }
        _board = game;
    }
}

/**
 * Reads the last play by the human, and moves accordingly
 */
void prunedAI::aiTakeNextTurn() {
    int last_play = 0;
    // Do not move to next node on the first turn
    if(_board->getOpenSpots() != BOARD_SIZE){
        // convert the play index to the branch index
        for (int i = 0; i < _board->getIndexOfLastPlay(); ++i) {
            last_play += _board->checkPlace(i) == EMPTY;
        }
        // advance to the next node based on the opponents play
        _game_state = _game_state->getBranch(last_play);
    }
    _board->setPlace(_game_state->getAiIndex(), _ai_mark);
}

/**
 * Recursively counts all nodes in the game tree
 * @return The number of nodes in the tree
 */
int prunedAI::countAllNodes() {
    return _tree_start->countNodes();
}

/**
 * !!WILL SEGFAULT!! Function to aid in debugging, requires a breakpoint to be placed
 */
void prunedAI::debug() {
    _tree_start->navigateTree();
}

// MOVE NODE

/**
 * Represents a single turn, contains the best position for the AI and all possible human outcomes
 * @param counters An array pointing to move_nodes for all possible human moves
 * @param len The length of the counter array
 * @param ai_i The index of the best possible AI play on the board
 */
prunedAI::move_node::move_node(move_node **counters, short len, short ai_i) {
    _counter_moves = counters;
    _counter_len = len;
    _ai_play_index = ai_i;
}

/**
 * Deallocates all sub-nodes in the counter array, as well as the array
 */
prunedAI::move_node::~move_node() {
    if(!hasNoCounters()) {
        for (int i = 0; i < _counter_len; ++i) {
            delete _counter_moves[i];
        }
        delete[] _counter_moves;
    }
}

/**
 * Store the node's data in a way to make it easy to visualize in the debugger
 */
void prunedAI::move_node::investigateNode() {
    move_node* counters[BOARD_SIZE];
    for(int i = 0; i < BOARD_SIZE; i++){
        if(i < _counter_len) {
            counters[i] = _counter_moves[i];
        } else {
            counters[i] = nullptr;
        }
    }
    return; // put a break point here
}

// NODE TREE

/**
 * Class responsible for managing all of the nodes
 */
prunedAI::node_tree::node_tree() {
    _head = nullptr;
    _ai_mark = EMPTY;
    _human_mark = EMPTY;
}

/**
 * Deallocates game tree, if it exists
 */
prunedAI::node_tree::~node_tree() {
    delete _head;
}

/**
 * Start recursively generating a game tree, with X as the AI
 */
void prunedAI::node_tree::generateTree() {
    int human_wins = 0;
    int ai_wins = 0;
    auto* b_state = new board();

    _ai_mark = X_MARK;
    _human_mark = O_MARK;
    _head = _rec_generate_game_node(b_state, ai_wins, human_wins);

}

/**
 * Recursively count all nodes inside the game tree
 * @return The number of nodes in the Tree
 */
int prunedAI::node_tree::countNodes() {
    return _rec_count_children(0, _head);
}

/**
 * Count all nodes on the level, and then recurse down until the bottom
 * @param level The current level that is being counted, starts at 0
 * @param node The node that's sub-nodes are counted
 * @return Count of node's sub-nodes including itself
 */
int prunedAI::node_tree::_rec_count_children(int level, move_node *node) {
    if(node->hasNoCounters()) return 1;
    int count = 0;
    for(int i = 0; i < BOARD_SIZE - level; i++) {
        count += _rec_count_children(level+1, node->getBranch(i));
    }
    return count+1;
}

/**
 * Creates a new game node to represent the state of the board, tries all possible plays and keeps the best
 * @param board A board containing the state of the game that the new node will represent
 * @param ai_wins Set to the number of winning states that the move will lead to
 * @param h_wins Set to the number of losing states that the move will lead to
 * @return pointer to the new node created
 */
prunedAI::move_node* prunedAI::node_tree::_rec_generate_game_node(board *board, int &ai_wins, int &h_wins) {
    if(!board->canPlay()) {
        // if human won update h_wins
        h_wins += board->checkWin() == _human_mark;
        return nullptr;
    }

    int b_h_wins = 0;
    int b_ai_wins = 0;
    float b_ratio = 0;
    move_node* b_node = nullptr;

    int t_h_wins;
    int t_ai_wins;
    for (char i = 0; i < BOARD_SIZE; ++i) {
        if(board->checkPlace(i) == EMPTY) {
            t_h_wins = t_ai_wins = 0;
            board->setPlace(i, _ai_mark);

            auto* t_node = new move_node(
                    _rec_generate_human_turns(board, t_ai_wins, t_h_wins),
                    board->getOpenSpots(),
                    i);

            board->erasePlay(i);

            // board either is full, or won
            if(t_node->hasNoCounters()) {
                h_wins += t_h_wins;
                ai_wins += t_ai_wins;
                delete b_node;
                return t_node;

            } else {
                // check to see if new nodes win/loss is better
                float t_ratio = (float)t_ai_wins / (float)t_h_wins;
                // make sure at least one move is chosen
                if(t_ratio > b_ratio || b_node == nullptr) {
                    b_h_wins = t_h_wins;
                    b_ai_wins = t_ai_wins;
                    b_ratio = (float)b_ai_wins / (float)b_h_wins;
                    std::swap(t_node, b_node);
                }
            }
            delete t_node;
        }
    }

    ai_wins += b_ai_wins;
    h_wins += b_h_wins;
    return b_node;
}

/**
 * Create the array of all possible human outcomes to the AI's move
 * @param board The board containing the current game state
 * @param ai_wins Set to the number of winning states that the move will lead to
 * @param h_wins Set to the number of losing states that the move will lead to
 * @return pointer to the array of counters
 */
prunedAI::move_node** prunedAI::node_tree::_rec_generate_human_turns(board *board, int &ai_wins, int &h_wins) {
    // check if board passed can play, or if the ai won
    if(!board->canPlay()) {
        ai_wins += board->checkWin() == _ai_mark;
        return nullptr;
    }

    int arr_len = board->getOpenSpots();
    auto** c_arr = new move_node *[arr_len];

    int c_index = 0;
    for (int i = 0; i < BOARD_SIZE; ++i) {
        if(board->checkPlace(i) == EMPTY) {
            board->setPlace(i, _human_mark);
            c_arr[c_index] = _rec_generate_game_node(board, ai_wins, h_wins);
            board->erasePlay(i);
            c_index++;
        }
        if(c_index == arr_len) break;
    }
    return c_arr;
}

/**
 * !WILL SEGFAULT! only useful in debugging to move throughout the tree
 */
[[noreturn]] void prunedAI::node_tree::navigateTree() {
    int level = 1;
    move_node* node = _head;
    move_node* counters[BOARD_SIZE];

    while (true) {
        for(int i = 0; i < BOARD_SIZE; i++) {
            if(i < BOARD_SIZE - level) {
                counters[i] = node->getBranch(i);
            } else {
                counters[i] = nullptr;
            }
        }

        int selection;
        std::cin >> selection;
        node = node->getBranch(selection);
        level += 2;
    }

}

