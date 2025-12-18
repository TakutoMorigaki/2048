#include "board.hpp"
#include "ai_eval.hpp"

// 最大値の検出
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

// 各行の値が連続しているかを検出
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
    // ver.4以降左上を最高値にするように
    // else if(max_grid(board) == board.grid[0][3]){
    //     for(int i = 0; i < 4; i++){
    //         bonus +=
    //             board.grid[i][3] * 200 +
    //             board.grid[i][2] * 100 +
    //             board.grid[i][1] * 50  +
    //             board.grid[i][0] * 1;
    //     }
    // }
    
    return bonus;
}

// 合体できる数の検出
int marge_bonus(board_2048 &board){
    int bonus = 0;

    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            if(j < 3 && board.grid[i][j] >= board.grid[i][j + 1]){
                bonus += board.grid[i][j];
            }
            if(i < 3 && board.grid[i][j] >= board.grid[i + 1][j]){
                bonus += board.grid[i][j];
            }
        }
    }

    return bonus;
}

// 動ける方向数
int mobility(board_2048 &board){
    int mobility = 0;
    if(CanMove_R(board)) mobility++;
    if(CanMove_L(board)) mobility++;
    if(CanMove_U(board)) mobility++;
    if(CanMove_D(board)) mobility++;
    return mobility;
}

// 左上もしくは右上(ver.4から左上)に最大値が来るようにする

// 評価関数ver.1
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

// 評価関数ver.2
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

// 評価関数ver.3
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

// 評価関数ver.4
int evaluate_board_04(board_2048 &board){
    int score = 0;

    score += board.vacant_total * 50;

    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            score += board.grid[i][j];
        }
    }

    if(board.grid[0][0] == max_grid(board)){
        score += 1000;
    }
    else {
        score -= 5000;
    }

    score += continuously(board);
    score += marge_bonus(board) * 50;

    return score;
}

// 評価関数ver.5
int evaluate_board_05(board_2048 &board){
    int score = 0;

    score += board.vacant_total * 50;

    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            score += board.grid[i][j];
        }
    }

    if(board.grid[0][0] == max_grid(board)){
        score += 1000;
    }
    else if(max_grid(board) >= 128 &&
            board.grid[0][0] != max_grid(board)){
        score -= 2000;
    }

    score += continuously(board);
    score += marge_bonus(board) * 50;
    score += mobility(board) * 100;

    return score;
}