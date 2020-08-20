//
// Created by ccone on 6/21/2020.
//

#include <iostream>

#include "board.h"

board::board() {
    for (auto & i : _g_board) {
        for (auto & j : i) {
            j = EMPTY;
        }
    }
    _open_spots = BOARD_SIZE;
}

board::~board() = default;

boardSpot board::checkWin() {
    bool r_diag_x_win = true;
    bool l_diag_x_win = true;
    bool r_diag_o_win = true;
    bool l_diag_o_win = true;

    for (int i = 0; i < BOARD_SIDE_LENGTH; ++i) {
        bool vert_x_win = true;
        bool horiz_x_win = true;
        bool vert_o_win = true;
        bool horiz_o_win = true;

        for (int j = 0; j < BOARD_SIDE_LENGTH; ++j) {
            horiz_x_win &= _g_board[i][j] == X_MARK;
            horiz_o_win &= _g_board[i][j] == O_MARK;
            vert_x_win &= _g_board[j][i] == X_MARK;
            vert_o_win &= _g_board[j][i] == O_MARK;
        }

        // check if there was a win
        if (vert_x_win || horiz_x_win) {
            return X_MARK;
        }
        else if (vert_o_win || horiz_o_win) {
            return O_MARK;
        }

        // two more checks for the diag
        r_diag_x_win &= _g_board[i][i] == X_MARK;
        r_diag_o_win &= _g_board[i][i] == O_MARK;

        l_diag_o_win &= _g_board[i][BOARD_SIDE_LENGTH - (i+1)] == O_MARK;
        l_diag_x_win &= _g_board[i][BOARD_SIDE_LENGTH - (i+1)] == X_MARK;

    }

    if (r_diag_x_win || l_diag_x_win) {
        return X_MARK;
    }
    else if (r_diag_o_win || l_diag_o_win) {
        return O_MARK;
    }
    else {
        return EMPTY;
    }
}

boardSpot board::checkWinVerbose() {
    bool r_diag_x_win = true;
    bool l_diag_x_win = true;
    bool r_diag_o_win = true;
    bool l_diag_o_win = true;

    for (int i = 0; i < BOARD_SIDE_LENGTH; ++i) {
        bool vert_x_win = true;
        bool horiz_x_win = true;
        bool vert_o_win = true;
        bool horiz_o_win = true;

        for (int j = 0; j < BOARD_SIDE_LENGTH; ++j) {
            horiz_x_win &= _g_board[i][j] == X_MARK;
            horiz_o_win &= _g_board[i][j] == O_MARK;
            vert_x_win &= _g_board[j][i] == X_MARK;
            vert_o_win &= _g_board[j][i] == O_MARK;
        }

        // check if there was a win
        if (vert_x_win || horiz_x_win) {
            std::cout << "single col X win" << std::endl;
            return X_MARK;
        }
        else if (vert_o_win || horiz_o_win) {
            return O_MARK;
        }

        // two more checks for the diag
        l_diag_x_win &= _g_board[i][i] == X_MARK;
        l_diag_o_win &= _g_board[i][i] == O_MARK;

        r_diag_o_win &= _g_board[i][BOARD_SIDE_LENGTH - (i+1)] == O_MARK;
        r_diag_x_win &= _g_board[i][BOARD_SIDE_LENGTH - (i+1)] == X_MARK;

    }

    if (r_diag_x_win || l_diag_x_win) {
        std::cout << "Diagonal X win" << std::endl;
        return X_MARK;
    }
    else if (r_diag_o_win || l_diag_o_win) {
        return O_MARK;
    }
    else {
        return EMPTY;
    }
}

bool board::setPlace(int r, int c, boardSpot mark) {
    if(_g_board[r][c] == EMPTY) {
        _g_board[r][c] = mark;
        --_open_spots;

        return true;
    }
    return false;
}

boardSpot board::checkPlace(int r, int c) {
    return _g_board[r][c];
}

boardSpot board::checkPlace(int i) {
    return _g_board[i/BOARD_SIDE_LENGTH][i%BOARD_SIDE_LENGTH];
}

bool board::setPlace(int i, boardSpot mark) {
    if(_g_board[i/BOARD_SIDE_LENGTH][i%BOARD_SIDE_LENGTH] == EMPTY) {
        _g_board[i/BOARD_SIDE_LENGTH][i%BOARD_SIDE_LENGTH] = mark;
        _open_spots--;
        return true;
    }
    return false;
}

bool board::canPlay() {
    return !(_open_spots == 0 || checkWin());
}

void board::erasePlay(int i) {
    if(_g_board[i/BOARD_SIDE_LENGTH][i%BOARD_SIDE_LENGTH] != EMPTY) {
        _g_board[i/BOARD_SIDE_LENGTH][i%BOARD_SIDE_LENGTH] = EMPTY;
        _open_spots++;
    }
}

