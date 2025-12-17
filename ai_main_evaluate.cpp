#include "board.hpp"
#include "ai_eval.hpp"
#include <iostream>
#include <fstream>
using namespace std;

Action choose_action(const board_2048 &board){
    int best_score = -1e9;
    Action best_action;

    for(int a = 0; a < 4; a++){
        board_2048 copy = board;
        Action act = static_cast<Action>(a);

        if(!step(copy, act)) continue;

        int score = evaluate_board_03(copy);

        if(score > best_score){
            best_score  = score;
            best_action = act;
        }
    }
    return best_action;
}


int main() {
    ofstream file("playdata_evaluate3.csv", ios::app);

    if(!file.is_open()){
        cerr << "file cannot open" << endl;
        return 1;
    }

    file << "STEPS,SCORE" << "\n";

    for(int i = 0; i < 1000; i++){
        board_2048 board;
        init_board(board);
        int steps = 0;

        while(!board.gameover_flg){
            Action action = choose_action(board);

            step(board, action);

            if(board.vacant_total == 0 &&
            !CanMove_R(board) &&
            !CanMove_L(board) &&
            !CanMove_U(board) &&
            !CanMove_D(board)){
                board.gameover_flg = true;
            }
            steps++;
        }

        int score = calc_score(board);
        file << steps << "," << score << "\n";
    }
    
    file.close();
    cout << "finished" << endl;

    return 0;
}