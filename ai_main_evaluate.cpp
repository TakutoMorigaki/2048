#include "board.hpp"
#include "ai_eval.hpp"
#include <iostream>
#include <fstream>
using namespace std;

bool Can_move(board_2048 &board, Action act){
    switch (act) {
        case ACT_RIGHT:
            if(CanMove_R(board)) return true;
            else return false;
        case ACT_LEFT:
            if(CanMove_L(board)) return true;
            else return false;
        case ACT_UP:
            if(CanMove_U(board)) return true;
            else return false;
        case ACT_DOWN:
            if(CanMove_D(board)) return true;
            else return false;
        default: return false;
    }
}

Action choose_action(board_2048 &board){
    int best_score = -1e9;
    Action best_action;
    for(int a = 0; a < 4; a++){
        board_2048 copy = board;
        Action act = static_cast<Action>(a);

        if(!Can_move(copy, act)) continue;
        int score;
        if(board.vacant_total > 0)
            score = evaluate_allpatern(copy);
        else
            score = evaluate_board_05(copy);

        if(score > best_score){
            best_score  = score;
            best_action = act;
        }
        // cout << a << endl;
    }
    // cout << best_score << endl;
    return best_action;
}


int main() {
    ofstream file("./Analyze/playdata_evaluate_all.csv", ios::app);

    if(!file.is_open()){
        cerr << "file cannot open" << endl;
        return 1;
    }

    file << "STEPS,SCORE" << "\n";

    for(int i = 0; i < 10000; i++){
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
        cout << i << endl;
        int score = calc_score(board);
        file << steps << "," << score << "\n";
    }
    
    file.close();
    cout << "finished" << endl;

    return 0;
}