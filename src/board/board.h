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

    void erasePlay(int i);

    bool setPlace(int i, boardSpot mark);
    bool setPlace(int r, int c, boardSpot mark);
    bool canPlay();

    boardSpot checkPlace(int i);
    boardSpot checkPlace(int r, int c);
    boardSpot checkWin();

    int getOpenSpots() const { return _open_spots; }

    bool operator==(const board& other);

private:
    boardSpot _g_board[BOARD_SIZE];
    int _open_spots;

    static const char _winning_combos[24];
};


#endif //TICTACTOES_BOARD_H
