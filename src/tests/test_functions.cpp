//
// Created by ccone on 7/24/2020.
//

#include "test_functions.h"
/**
 * ABOUT:
 *  These were quick and messy functions I wrote throughout creating this program to test
 *  out certain aspects of my code and c++
 */

void testWinValidation() {
    board row_b;
    board col_b;
    board diag_r_b;
    board diag_l_b;

    for (int i = 0; i < BOARD_SIDE_LENGTH; ++i) {
        row_b.setPlace(i, 0, X_MARK);
        col_b.setPlace(0, i, X_MARK);
        diag_r_b.setPlace(i, i, X_MARK);
        diag_l_b.setPlace(BOARD_SIDE_LENGTH - (i+1), BOARD_SIDE_LENGTH - (i+1), X_MARK);
    }


    std::cout << "TESTING: row board: ";
    if(row_b.checkWin() == X_MARK) {
        std::cout << "SUCCESS" << std::endl;
    }

    std::cout << "TESTING: col board: ";
    if(col_b.checkWin() == X_MARK) {
        std::cout << "SUCCESS" << std::endl;
    }

    std::cout << "TESTING: r_diag board: ";
    if(diag_r_b.checkWin() == X_MARK) {
        std::cout << "SUCCESS" << std::endl;
    }

    std::cout << "TESTING: l_diag board: ";
    if(diag_l_b.checkWin() == X_MARK) {
        std::cout << "SUCCESS" << std::endl;
    }

    ez_t::timer t(ez_t::interval::Millis);
    diag_l_b.checkWin();
    std::cout << "took " << t.getTime() << "millis to check" << std::endl;

}

void testTreeBuild() {
    dynamicCounterAI gameTree;
    gameTree.generateTree();

    std::cout << "There is " << gameTree.getXCount() << " ways for X to win" << std::endl;
    std::cout << "There is " << gameTree.getOCount() << " ways for O to win" << std::endl;
    std::cout << "Tree has " << gameTree.countAllNodes() << " total nodes" << std::endl;

    int tot_nodes = 0;
    for (int i = 0; i < 12; ++i) {
        int nodes_at_level = gameTree.countNodesAtLevel(i);
        std::cout << "On level " << i << " there is " << nodes_at_level << " node(s)" << std::endl;
        tot_nodes += nodes_at_level;
    }
    std::cout << "counted " << tot_nodes << "   nodes total" << std::endl;
}

void testGameBoard() {
    gameBoard game_board;

    int i = 0;
    while (i < 5) {
        int row_choice = 0;
        int col_choice = 0;

        std::cout << "row:";
        std::cin >> row_choice;
        std::cout << "col:";
        std::cin >> col_choice;

        if(game_board.setPlace(row_choice, col_choice, X_MARK)) {
            game_board.printBoard();
            std::cout << std::endl;
            i++;
        } else {
            std::cout << "Space is already occupied" << std::endl;
        }
    }

    switch(game_board.checkWin()) {
        case EMPTY:
            std::cout << "It's a Draw!" << std::endl;
            break;
        case X_MARK:
            std::cout << "X Wins!" << std::endl;
            break;
        case O_MARK:
            std::cout << "O Wins!" << std::endl;
            break;
    }
}

void testAICreation() {
    boardSpot ai_mark;
    boardSpot player_mark;

    opponentAI* dynamic_ai = new dynamicCounterAI();
    opponentAI* minmax_ai = new prunedAI();

    ai_mark = X_MARK;

    // test generation
    ez_t::timer m(ez_t::interval::Millis);
    dynamic_ai->generateTree();
    std::cout << "Took " << m.getTime() << " millis to create the non-pruned ai tree" << std::endl;
    // probably will not work
    ez_t::timer a(ez_t::interval::Millis);
    minmax_ai->generateTree();
    std::cout << "Took " << a.getTime() << " millis to create the pruned ai tree" << std::endl;

    delete dynamic_ai;
    delete minmax_ai;
    std::cout << "done" << std::endl;
}

void testObjects() {
    board game_board;
    game_board.setPlace(4, X_MARK);
}

void testTimerLibrary() {
    ez_t::timer m(ez_t::interval::Millis);
    ez_t::timer s(ez_t::interval::Seconds);
    ez_t::timer mi(ez_t::interval::Minutes);

    for(int i = 0; i < 100000; i++) {
        if( i % 4 == 0 ) {
            std::cout << i << std::endl;
        }
    }

    std::cout << "Took " << m.getTime() << " Millis ";
    std::cout << s.getTime() << " Seconds ";
    std::cout << mi.getTime() << " Minutes " << std::endl;

}

void testDebugger() {
    std::vector<int> v;
    for (int i = 0; i < 5; ++i) {
        v.push_back(i);
    }

}

void testFloats() {
    float zeroDivZero = 0 / 0.0;
    std::cout << "The result of 0 / 0.0 is: " << zeroDivZero << std::endl;

    bool lessThan = zeroDivZero < 0;
    std::cout << "Zero div zero is less than zero:  " << lessThan << std::endl;

    float clampTest = zeroDivZero * lessThan;
    std::cout << "the result of the clamp is: " << clampTest << std::endl;

    bool lessThanOther = zeroDivZero < 56;
    //std::cout
}

void testDynamicArrays() {
    int * arr;
    arr = new int[78];
    delete [] arr;

    arr = new int[0];
    delete [] arr;
}

void testArrayDelete() {
    class a {
    public:
        a() = default;
        ~a() {
            std::cout << "called destructor" << std::endl;
        }
    };

    a** arr;
    arr = new a*[10];

    for(int i = 0; i < 10; i++) {
        arr[i] = new a();
    }

    for(int i = 0; i < 10; i++) {
        delete arr[i];
    }

    delete [] arr;
}

void testGamePlay() {
    opponentAI* test_ai = nullptr;
    bool getting_choice = true;
    while(getting_choice) {
        int c = 0;
        std::cout << "which ai iteration do you want to test: " << std::endl;
        std::cout << "0: dynamic size counter arrays" << std::endl;
        std::cout << "1: alpha-beta pruned tree" << std::endl;

        std::cin >> c;
        switch (c) {
            case 0:
                test_ai = new dynamicCounterAI();
                getting_choice = false;
                break;

            case 1:
                test_ai = new prunedAI();
                getting_choice = false;
                break;

            default:
                std::cout << c << " is an invalid choice" << std::endl;
        }
    }

    test_ai->generateTree();

    gameBoard* g_board = new gameBoard();
    test_ai->startGame(g_board);

    boardSpot turn_mark = X_MARK;
    while (g_board->canPlay()) {
        if(turn_mark == X_MARK) {
            test_ai->aiTakeNextTurn();
        } else {
            g_board->promptUserInput(O_MARK);
        }

        // move over to opposite turn
        switch (turn_mark) {
            case X_MARK:
                turn_mark = O_MARK;
                break;
            default:
                turn_mark = X_MARK;
                break;
        }

        g_board->printBoard();
        std::cout << std::endl;
    }

    switch (g_board->checkWin()) {
        case X_MARK:
            std::cout << "X WINS" << std::endl;
            break;
        case O_MARK:
            std::cout << "O WINS" << std::endl;
            break;
        case EMPTY:
            std::cout << "DRRRAAAWWWW" << std::endl;
            break;
    }

    delete g_board;
    delete test_ai;
}

void debugTree() {
    auto* tree = new prunedAI();
    tree->generateTree();
//    tree->debug();

//    dynamicCounterAI tree;
//    tree.generateTree(X_MARK);
//    tree.printWinGrid();

    delete tree;
}