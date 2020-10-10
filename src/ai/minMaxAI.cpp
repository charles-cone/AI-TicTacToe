//
// Created by ccone on 9/21/2020.
//

#include "minMaxAI.h"

// PRUNE AI

minMaxAI::minMaxAI() {
    _tree_start = nullptr;
    _game_state = nullptr;
    _ai_mark = EMPTY;
}

minMaxAI::~minMaxAI() {
    delete _tree_start;
}

void minMaxAI::generateTree(boardSpot ai) {
    // TODO erase old tree before making a new one
    if(_tree_start == nullptr && ai != _ai_mark ) {
        _ai_mark = ai;
        _tree_start = new node_tree();
        _tree_start->generateTree(_ai_mark);
    }
}

void minMaxAI::startGame(gameBoard *game) {
    if(_tree_start != nullptr) {
        if(_ai_mark == X_MARK) {
            _game_state = _tree_start->getHead();
        }
        _board = game;
    }
}

void minMaxAI::aiTakeNextTurn() {
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
    // TODO get rid of debugging
    //_game_state->investigateNode();
    _board->setPlace(_game_state->getAiIndex(), _ai_mark);
}

// TODO rename this to make some aspect of it less shitty
int minMaxAI::countChildren() {
    return _tree_start->countChildren();
}

long int minMaxAI::getTreeSize() {
    return 0;
}

void minMaxAI::debug() {
    _tree_start->navigateTree();
}

// MOVE NODE

minMaxAI::move_node::move_node(move_node **counters, int len, char ai_i) {
    _counter_moves = counters;
    _counter_len = len;
    _ai_play_index = ai_i;
}

minMaxAI::move_node::~move_node() {
    if(!hasNoCounters()) {
        for (int i = 0; i < _counter_len; ++i) {
            delete _counter_moves[i];
        }
        delete[] _counter_moves;
    }
}

// store the node's data in a way to make it easy to visualize in the debugger
void minMaxAI::move_node::investigateNode() {
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

minMaxAI::node_tree::node_tree() {
    _head = nullptr;
    _ai_mark = EMPTY;
    _human_mark = EMPTY;
}

minMaxAI::node_tree::~node_tree() {
    delete _head;
}

void minMaxAI::node_tree::generateTree(boardSpot ai) {
    int human_wins = 0;
    int ai_wins = 0;
    auto* b_state = new board();

    switch (ai) {
        case X_MARK:
            _ai_mark = X_MARK;
            _human_mark = O_MARK;
            generateEdgeCaseBoard();
            _head = _rec_generate_game_node(b_state, ai_wins, human_wins);
            break;
        case O_MARK:
            _ai_mark = O_MARK;
            _human_mark = X_MARK;
            break;
        default:
            break;
    }
    delete b_state;
}

int minMaxAI::node_tree::countChildren() {
    return _rec_count_children(0, nullptr);
}


int minMaxAI::node_tree::_rec_count_children(int level, move_node *node) {
    if(node->hasNoCounters()) return 1;
    int count = 0;
    for(int i = 0; i < BOARD_SIZE - level; i++) {
        count += _rec_count_children(level+1, node->getBranch(i));
    }
    return count+1;
}

minMaxAI::move_node* minMaxAI::node_tree::_rec_generate_game_node(board *board, int &ai_wins, int &h_wins) {
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
    //TODO move this down to lower scope
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
                float t_ratio = (float)t_ai_wins / (float)t_h_wins;
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

minMaxAI::move_node** minMaxAI::node_tree::_rec_generate_human_turns(board *board, int &ai_wins, int &h_wins) {
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

[[noreturn]] void minMaxAI::node_tree::navigateTree() {
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

void minMaxAI::node_tree::generateEdgeCaseBoard() {
    board f;
    f.setPlace(8, X_MARK);
    f.setPlace(0, O_MARK);
    f.setPlace(7, X_MARK);
    f.setPlace(6, O_MARK);
    f.setPlace(5, X_MARK);
    //f.setPlace(5, O_MARK);
    _d_board = f;
}

