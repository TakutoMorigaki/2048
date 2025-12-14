#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <random>
using namespace std;

// 2048のボード構造体
struct board_2048 {
    int grid[4][4];
    bool gameover_flg;
    bool move[4];
    bool move_r_flg[4][4];
    bool move_l_flg[4][4];
    bool move_u_flg[4][4];
    bool move_d_flg[4][4];
    bool marge_r_flg[4][4];
    bool marge_l_flg[4][4];
    bool marge_u_flg[4][4];
    bool marge_d_flg[4][4];
};

// 乱数生成器の初期化
random_device rd;   // ハードウェア乱数生成機
mt19937 mt(rd());   // メルセンヌ・ツイスタ法の生成器
// 0~4までの一様分布の乱数を生成
uniform_int_distribution<int> dist1(0, 3);
// 1か2の一様分布の乱数を生成
uniform_int_distribution<int> dist2(1, 2);
char direction;

// ボードの初期化
void init_board(board_2048 *board){

    // グリッドの初期化
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            board->grid[i][j] = 0;
        }
    }

    // 最初に値を生成するマスの座標を生成
    int x = dist1(mt);
    int y = dist1(mt);
    // 生成する値の決定
    int value = dist2(mt);
    board->grid[x][y] = value;

    // 各種フラグの初期化
    board->gameover_flg = false;

    for(int i = 0; i < 4; i++){
        board->move[i] = true;
    }

    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            // どこも合成できない
            board->marge_r_flg[i][j] = false;
            board->marge_l_flg[i][j] = false;
            board->marge_u_flg[i][j] = false;
            board->marge_d_flg[i][j] = false;

            if(board->grid[i][j] != 0){
                // 上下左右端以外なら全方向動かせる
                if(i != 0 && i != 3 && j != 0 && j != 0){
                    board->move_r_flg[i][j] = true;
                    board->move_l_flg[i][j] = true;
                    board->move_u_flg[i][j] = true;
                    board->move_d_flg[i][j] = true;
                }
                else{
                    // 上下端は動かせない
                    if(i == 0){
                        board->move_u_flg[i][j] = false;
                        board->move_d_flg[i][j] = true;
                    }
                    else if(i == 3){
                        board->move_u_flg[i][j] = true;
                        board->move_d_flg[i][j] = false;
                    }
                    // 左右端は動かせない
                    if(j == 0){
                        board->move_r_flg[i][j] = true;
                        board->move_l_flg[i][j] = false;
                    }
                    else if(j == 3){
                        board->move_r_flg[i][j] = false;
                        board->move_l_flg[i][j] = true;
                    }
                }
            }
            // 値を持たないところは動けないで初期化
            else{
                board->move_r_flg[i][j] = false;
                board->move_l_flg[i][j] = false;
                board->move_u_flg[i][j] = false;
                board->move_d_flg[i][j] = false;
            }
        }
    }
}

// 一つのマスにおいて上下左右に動けるかを判定する
void CanMove_grid(board_2048 *board){
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            if(board->grid[i][j] != 0){
                if(
                    (board->grid[i][j] == board->grid[i][j + 1] || board->grid[i][j + 1] == 0)
                    && j != 3
                )       board->move_r_flg[i][j] = true;     // 右隣が０または同じ数字のとき右に動ける
                else    board->move_r_flg[i][j] = false;    // それ以外なら右に動けない

                if(
                    (board->grid[i][j] == board->grid[i][j - 1] || board->grid[i][j - 1] == 0)
                    && j != 0
                )       board->move_l_flg[i][j] = true;     // 左隣が０または同じ数字のときに左に動ける
                else    board->move_l_flg[i][j] = false;    // それ以外なら左に動けない

                if(
                    (board->grid[i][j] == board->grid[i - 1][j] || board->grid[i - 1][j] == 0)
                    && i != 0
                )       board->move_u_flg[i][j] = true;     // 上隣が０または同じ数字のときに上に動ける
                else    board->move_u_flg[i][j] = false;    // それ以外なら上に動けない

                if(
                    (board->grid[i][j] == board->grid[i + 1][j] || board->grid[i + 1][j] == 0)
                    && i != 3
                )       board->move_d_flg[i][j] = true;     // 下隣が０または同じ数字のときに下に動ける
                else    board->move_d_flg[i][j] = false;    // それ以外なら下に動けない
            }
        }
    }
}

// 右に動けるか判定(true:動ける, false:動けない)
bool CanMove_R(board_2048 *board){
    for(int i = 0; i < 4; i++){
        if(
            board->move_r_flg[i][0] ||
            board->move_r_flg[i][1] ||
            board->move_r_flg[i][2]
        ) return true;
    }
    return false;
}

// 左に動けるか判定(true:動ける, false:動けない)
bool CanMove_L(board_2048 *board){
    for(int i = 0; i < 4; i++){
        if(
            board->move_l_flg[i][1] ||
            board->move_l_flg[i][2] ||
            board->move_l_flg[i][3]
        ) return true;
    }
    return false;
}

// 上に動けるか判定(true:動ける, false:動けない)
bool CanMove_U(board_2048 *board){
    for(int i = 0; i < 3; i++){
        if(
            board->move_u_flg[1][i] ||
            board->move_u_flg[2][i] ||
            board->move_u_flg[3][i]
        ) return true;
    }
    return false;
}

// 下に動けるか判定(true:動ける, false:動けない)
bool CanMove_D(board_2048 *board){
    for(int i = 0; i < 3; i++){
        if(
            board->move_d_flg[0][i] ||
            board->move_d_flg[1][i] ||
            board->move_d_flg[2][i]
        ) return true;
    }
    return false;
}

void swap(board_2048 *board, int i, int j, char c){
    if(c == 'r'){
        board->grid[i][j] = board->grid[i][j + 1];
        board->grid[i][j] = 0;
    }
    else if(c == 'l'){
        board->grid[i][j] = board->grid[i][j - 1];
        board->grid[i][j] = 0;
    }
    else if(c == 'u'){
        board->grid[i][j] = board->grid[i - 1][j];
        board->grid[i][j] = 0;
    }
    else{
        board->grid[i][j] = board->grid[i + 1][j];
        board->grid[i][j] = 0;
    }
}

void marge(board_2048 *board, int i, int j, char c){
    if(c == 'r'){
        board->grid[i][j + 1] << 1;
        board->grid[i][j] = 0;
    }
    else if(c == 'l'){
        board->grid[i][j - 1] << 1;
        board->grid[i][j] = 0;
    }
    else if(c == 'u'){
        board->grid[i - 1][j] << 1;
        board->grid[i][j] = 0;
    }
    else{
        board->grid[i + 1][j] << 1;
        board->grid[i][j] = 0;
    }
}

void Move_R(board_2048 *board){
    for(int i = 0; i < 4; i++){
        for(int j = 2; j >= 0; j--){
            if(board->grid[i][j] != 0){
                if(board->grid[i][j] == board->grid[i][j + 1]){
                    board->grid[i][j + 1] << 1; // 二倍をシフト演算で表現
                    board->grid[i][j] = 0;
                }
                else if(board->grid[i][j + 1] == 0){
                    board->grid[i][j + 1] = board->grid[i][j];
                    board->grid[i][j] = 0;
                }
            }
        }
    }
}

// ボードの表示
void print_board(board_2048 *board){
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            if(board->grid[i][j] == 0){
                cout << setfill(' ') << setw(6) << ". ";
            }
            else{
                cout << setfill('0') << setw(5) << board->grid[i][j] << " ";
            }
        }
        cout << endl;
    }
}

// スコアの計算
int calc_score(board_2048 *board){
    int score = 0;
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            score += board->grid[i][j];
        }
    }
    return score;
}


int main(){
    board_2048 *board = new board_2048;
    init_board(board);
    print_board(board);
    while(!board->gameover_flg){
        CanMove_grid(board);
        if(CanMove_R(board) || CanMove_L(board) || CanMove_U(board) || CanMove_D(board)){
            cout << "select direction(w:↑, a:←, s:↓, d:→)" << endl;
            cout << "→:" << CanMove_R(board) << endl;
            cout << "←:" << CanMove_L(board) << endl;
            cout << "↑:" << CanMove_U(board) << endl;
            cout << "↓:" << CanMove_D(board) << endl;
            cin >> direction; 
            if(direction == 'w'){
                if(!CanMove_U(board)){
                    cout << "Invaild direction" << endl << endl;
                }
                else{

                }
            }
            else if(direction == 'a'){
                if(!CanMove_L(board)){
                    cout << "Invaild direction" << endl << endl;
                }
                else{
                    
                }
            }
            else if(direction == 's'){
                if(!CanMove_D(board)){
                    cout << "Invaild direction" << endl << endl;
                }
                else{
                    
                }
            }
            else if(direction == 'd'){
                if(!CanMove_R(board)){
                    cout << "Invaild direction" << endl << endl;
                }
                else{
                    
                }
            }
            else cout << "Invaild key" << endl << endl;

            print_board(board);
        }
        else board->gameover_flg = true;
    }
    cout << "gameover" << endl;
    cout << "SCORE: " << calc_score(board);
    return 0;
}