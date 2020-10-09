//
// Created by ccone on 6/21/2020.
//

#include "board.h"

board::board() {
    for (auto & i : _g_board) {
        i = EMPTY;
    }
    _open_spots = BOARD_SIZE;
}

board::~board() = default;

// I did not come up with this method of win validation, I believe this is where I found it
// Credit to https://stackoverflow.com/questions/22488100/find-winner-in-a-tic-tac-toe-match
const char board::_winning_combos[24] = {
        // Vertical wins
        0, 1, 2,
        3, 4, 5,
        6, 7, 8,
        // Horizontal wins
        0, 3, 6,
        1, 4, 7,
        2, 5, 8,
        // Diagonal wins
        0, 4, 8,
        2, 4, 6
};

boardSpot board::checkWin() {
    for(int i = 0; i < 8; i++) {
        int x = i*3;
        boardSpot mark = _g_board[_winning_combos[x]];
        if(mark == EMPTY) continue;

        else if (mark == _g_board[_winning_combos[x+1]]
            && mark == _g_board[_winning_combos[x+2]]) return mark;
    }
    return EMPTY;
}


bool board::setPlace(int r, int c, boardSpot mark) {
    if(_g_board[r*3 + c] == EMPTY) {
        _g_board[r*3 + c] = mark;
        --_open_spots;
        return true;
    }
    return false;
}

boardSpot board::checkPlace(int r, int c) {
    return _g_board[r*3 + c];
}

boardSpot board::checkPlace(int i) {
    return _g_board[i];
}

bool board::setPlace(int i, boardSpot mark) {
    if(_g_board[i] == EMPTY) {
        _g_board[i] = mark;
        _open_spots--;
        return true;
    }
    return false;
}

bool board::canPlay() {
    return !(_open_spots == 0 || checkWin() != EMPTY);
}

void board::erasePlay(int i) {
    if(_g_board[i] != EMPTY) {
        _g_board[i] = EMPTY;
        _open_spots++;
    }
}

// Used in debugging to test out a couple of edge cases
bool board::operator==(const board &other) {
    bool eq = true;
    for(int i = 0; i < BOARD_SIZE; i++) {
        eq &= (_g_board[i] == other._g_board[i]);
        i += (!eq)*BOARD_SIZE; // break out of for loop once false is found
    }
    return eq;
 }

