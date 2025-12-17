#include "board.hpp"
#include "ai_eval.hpp"

int max_grid(board_2048 &board){
    int max = 0;
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            if(max < board.grid[i][j]){
                max = board.grid[i][j];
            }
        }
    }
    return max;
}

int continuously(board_2048 &board){
    int bonus = 0;

    if(max_grid(board) == board.grid[0][0]){
        for(int i = 0; i < 4; i++){
            bonus +=
                board.grid[i][0] * 200 +
                board.grid[i][1] * 100 +
                board.grid[i][2] * 50  +
                board.grid[i][3] * 1;
        }
    }
    else if(max_grid(board) == board.grid[0][3]){
        for(int i = 0; i < 4; i++){
            bonus +=
                board.grid[i][3] * 200 +
                board.grid[i][2] * 100 +
                board.grid[i][1] * 50  +
                board.grid[i][0] * 1;
        }
    }
    
    return bonus;
}

int evaluate_board_01(board_2048 &board){
    int score = 0;

    score += board.vacant_total * 50;

    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            score += board.grid[i][j];
        }
    }

    return score;
}

int evaluate_board_02(board_2048 &board){
    int score = 0;

    score += board.vacant_total * 50;

    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            score += board.grid[i][j];
        }
    }

    if(board.grid[0][0] == max_grid(board) ||
       board.grid[0][3] == max_grid(board)){
        score += 200;
    }

    return score;
}

int evaluate_board_03(board_2048 &board){
    int score = 0;

    score += board.vacant_total * 50;

    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            score += board.grid[i][j];
        }
    }

    if(board.grid[0][0] == max_grid(board) ||
       board.grid[0][3] == max_grid(board)){
        score += 1000;
    }

    score += continuously(board);

    return score;
}