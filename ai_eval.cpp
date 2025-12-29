#include "board.hpp"
#include "ai_eval.hpp"
#include <iostream>

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

// 空白のマスの二次元座標を配列に入れて返す(何個あるかの個数も返す)
void search_vacant(const board_2048 &board, int vacant_xy[][2], int &count){
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            if(board.grid[i][j] == 0){
                vacant_xy[count][0] = i;
                vacant_xy[count][1] = j;
                count++;
            }
        }
    }
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

// 評価関数を1,2が出現するときの両方の平均をとって、さらにどのマスに出現するかを考慮して空きマスすべてを検討するつもり
// 結果大失敗。要検討
int evaluate_allpatern(board_2048 &board){
    int score_1;        // 1が出現したときのスコア
    int score_2;        // 2が出現したときのスコア
    int avg;            // score_1,2の平均
    int best_score = -1e9;

    int vacant_xy[16][2] = {};
    int count = 0;
    search_vacant(board, vacant_xy, count);

    for(int i = 0; i < count; i++){
        int x = vacant_xy[i][0];
        int y = vacant_xy[i][1];
        board.grid[x][y] = 1;
        score_1 = evaluate_board_05(board);

        board.grid[x][y] = 2;
        score_2 = evaluate_board_05(board);

        avg = (score_1 + score_2) / 2;
        if(best_score < avg){
            best_score = avg;
        }
        board.grid[x][y] = 0;
    }
    return best_score;
}