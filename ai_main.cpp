#include "board.hpp"
#include <stdio.h>
#include <iostream>
#include <fstream>
using namespace std;

bool is_file_empty(const std::string& filename) {
    std::ifstream file(filename);
    return file.peek() == std::ifstream::traits_type::eof();
}

int main(){
    ofstream file("playdata.csv", ios::app);
    bool empty = is_file_empty("palydata.csv");

    if(!file.is_open()){
        cerr << "file cannot open" << endl;
        return 1;
    }

    // if(empty){
    //     file << "STEPS,SCORE" << "\n";
    // }

    for(int i = 0; i < 100; i++){

        board_2048 board;
        init_board(board);

        int steps = 0;
        
        while (!board.gameover_flg) {
            Action action = static_cast<Action>(ai_direc_random());
            
            if (board.vacant_total == 0 &&
                !CanMove_U(board) &&
                !CanMove_D(board) &&
                !CanMove_L(board) &&
                !CanMove_R(board)) {
                board.gameover_flg = true;
            }

            step(board, action);

            steps++;
        }
        int score = calc_score(board);

        cout << "Game over" << endl;
        cout << "steps = " << steps << endl;
        cout << "score = " << score << endl;

        file << steps << "," << score << "\n";
    }

    file.close();
    cout << "csv file has made" << endl;

    return 0;
}