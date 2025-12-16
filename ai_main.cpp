#include "board.hpp"
#include <stdio.h>
#include <iostream>
using namespace std;

int main(){
    board_2048 *board = new board_2048;
    init_board(board);

    int steps = 0;

    while (!board->gameover_flg) {
        Action action = static_cast<Action>(ai_direc());
        
        if (board->vacant_total == 0 &&
            !CanMove_U(board) &&
            !CanMove_D(board) &&
            !CanMove_L(board) &&
            !CanMove_R(board)) {
            board->gameover_flg = true;
        }

        step(board, action);

        steps++;
    }
    cout << "Game over" << endl;
    cout << "steps = " << steps << endl;
    cout << "score = " << calc_score(board) << endl;

    delete board;
    return 0;
}