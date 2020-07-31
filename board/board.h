//
// Created by ccone on 6/21/2020.
//

#ifndef TICTACTOES_BOARD_H
#define TICTACTOES_BOARD_H

#define BOARD_SIDE_LENGTH       3
#define BOARD_SIZE              9
#define MIN_TURNS_TO_WIN        5

enum boardSpot {
    EMPTY,
    X_MARK,
    O_MARK
};

class board {
public:
    board();
    ~board();

    bool setPlace(int i, boardSpot mark);
    bool setPlace(int r, int c, boardSpot mark);
    bool canPlay();

    boardSpot checkPlace(int i);
    boardSpot checkPlace(int r, int c);
    boardSpot checkWin();
    boardSpot checkWinVerbose();

    int getOpenSpots() { return _open_spots; }

private:
    boardSpot _g_board[BOARD_SIDE_LENGTH][BOARD_SIDE_LENGTH];
    int _open_spots;
};


#endif //TICTACTOES_BOARD_H
