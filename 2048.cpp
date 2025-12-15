#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <random>
using namespace std;

// 2048のボード構造体
struct board_2048 {
    unsigned int grid[4][4];    // ボードを表す4x4の二次元配列
    int vacant_total;           // 現在の空きマスの個数
    bool gameover_flg;          // ゲームオーバー判定フラグ
    bool move_r_flg[4][4];      // あるマスにおいて右に動けるかの判定フラグ
    bool move_l_flg[4][4];      // あるマスにおいて左に動けるかの判定フラグ
    bool move_u_flg[4][4];      // あるマスにおいて上に動けるかの判定フラグ
    bool move_d_flg[4][4];      // あるマスにおいて下に動けるかの判定フラグ
    bool marged_flg[4][4];      // すでに合成がなされているかを表すフラグ
};

// 乱数生成器の初期化
random_device rd;   // ハードウェア乱数生成機
mt19937 mt(rd());   // メルセンヌ・ツイスタ法の生成器
// 0~3までの一様分布の乱数を生成
uniform_int_distribution<int> dist1(0, 3);
// 1か2の一様分布の乱数を生成
uniform_int_distribution<int> dist2(1, 2);

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
        for(int j = 0; j < 4; j++){
            // はじめは合成されていない
            board->marged_flg[i][j] = false;
            if(board->grid[i][j] != 0){
                // 上下端は動かせない
                if(i == 0){
                    board->move_u_flg[i][j] = false;
                    board->move_d_flg[i][j] = true;
                }
                else if(i == 3){
                    board->move_u_flg[i][j] = true;
                    board->move_d_flg[i][j] = false;
                }
                else {
                    board->move_u_flg[i][j] = true;
                    board->move_d_flg[i][j] = true;
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
                else {
                    board->move_r_flg[i][j] = true;
                    board->move_l_flg[i][j] = true;
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

    board->vacant_total = 15;
}

// 一つのマスにおいて上下左右に動けるかを判定する
void CanMove_grid(board_2048 *board){
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            if(board->grid[i][j] != 0){
                if(
                    j != 3 &&
                    (board->grid[i][j] == board->grid[i][j + 1] ||
                     board->grid[i][j + 1] == 0)
                )       board->move_r_flg[i][j] = true;     // 右隣が０または同じ数字のとき右に動ける
                else    board->move_r_flg[i][j] = false;    // それ以外なら右に動けない

                if(
                    j != 0 &&
                    (board->grid[i][j] == board->grid[i][j - 1] || board->grid[i][j - 1] == 0)
                )       board->move_l_flg[i][j] = true;     // 左隣が０または同じ数字のときに左に動ける
                else    board->move_l_flg[i][j] = false;    // それ以外なら左に動けない

                if(
                    i != 0 &&
                    (board->grid[i][j] == board->grid[i - 1][j] || board->grid[i - 1][j] == 0)
                )       board->move_u_flg[i][j] = true;     // 上隣が０または同じ数字のときに上に動ける
                else    board->move_u_flg[i][j] = false;    // それ以外なら上に動けない

                if(
                    i != 3 &&
                    (board->grid[i][j] == board->grid[i + 1][j] || board->grid[i + 1][j] == 0)
                )       board->move_d_flg[i][j] = true;     // 下隣が０または同じ数字のときに下に動ける
                else    board->move_d_flg[i][j] = false;    // それ以外なら下に動けない
            }
            else{
                board->move_r_flg[i][j] = false;
                board->move_l_flg[i][j] = false;
                board->move_u_flg[i][j] = false;
                board->move_d_flg[i][j] = false;
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
    for(int i = 0; i < 4; i++){
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
    for(int i = 0; i < 4; i++){
        if(
            board->move_d_flg[0][i] ||
            board->move_d_flg[1][i] ||
            board->move_d_flg[2][i]
        ) return true;
    }
    return false;
}

// 座標(i, j)の値をkだけ上下左右に移動する
void move(board_2048 *board, int i, int j, int k, char c){
    if(c == 'r'){
        board->grid[i][j + k] = board->grid[i][j];
        board->grid[i][j] = 0;
    }
    else if(c == 'l'){
        board->grid[i][j - k] = board->grid[i][j];
        board->grid[i][j] = 0;
    }
    else if(c == 'u'){
        board->grid[j - k][i] = board->grid[j][i];
        board->grid[j][i] = 0;
    }
    else{
        board->grid[j + k][i] = board->grid[j][i];
        board->grid[j][i] = 0;
    }
}

// 座標(i, j)の値とkだけ移動させた場所にある値を合成する
void marge(board_2048 *board, int i, int j, int k, char c){
    if(c == 'r'){
        board->grid[i][j] <<= 1;
        move(board, i, j, k, c);
        board->marged_flg[i][j + k] = true;
    }
    else if(c == 'l'){
        board->grid[i][j] <<= 1;
        move(board, i, j, k, c);
        board->marged_flg[i][j - k] = true;
    }
    else if(c == 'u'){
        board->grid[j][i] <<= 1;
        move(board, i, j, k, c);
        board->marged_flg[j - k][i] = true;
    }
    else{
        board->grid[j][i] <<= 1;
        move(board, i, j, k, c);
        board->marged_flg[j + k][i] = true;
    }
    board->vacant_total++;
}

// ボードを右に動かす
void Move_R(board_2048 *board){
    for(int i = 0; i < 4; i++){
        for(int j = 2; j >= 0; j--){
            if(board->grid[i][j] != 0){
                for(int k = 1; j + k < 4; k++){
                    if(board->grid[i][j + k] != 0 &&
                       board->grid[i][j] == board->grid[i][j + k]){
                        if(board->marged_flg[i][j + k]){
                            // 合成済みのマスなら隣に移動
                            move(board, i, j, k - 1, 'r');
                            break;
                        }
                        else{
                            // 合成済みでなければそのまま合成
                            marge(board, i, j, k, 'r');
                            break;
                        }
                    }
                    else if(board->grid[i][j + k] != 0 &&
                            board->grid[i][j] != board->grid[i][j + k]){
                        if(k > 1){
                            move(board, i, j, k - 1, 'r');
                            break;
                        }
                        else {
                            break;
                        }
                    }
                    else if(j + k == 3){
                        move(board, i, j, k, 'r');
                    }
                }
            }
        }
    }
}

// ボードを左に動かす
void Move_L(board_2048 *board){
    for(int i = 0; i < 4; i++){
        for(int j = 1; j < 4; j++){
            if(board->grid[i][j] != 0){
                for(int k = 1; j - k >= 0; k++){
                    if(board->grid[i][j - k] != 0 &&
                       board->grid[i][j] == board->grid[i][j - k]){
                        if(board->marged_flg[i][j - k]){
                            // 合成済みのマスなら隣に移動
                            move(board, i, j, k - 1, 'l');
                            break;
                        }
                        else{
                            // 合成済みでなければそのまま合成
                            marge(board, i, j, k, 'l');
                            break;
                        }
                    }
                    else if(board->grid[i][j - k] != 0 &&
                            board->grid[i][j] != board->grid[i][j - k]){
                        if(k > 1){
                            move(board, i, j, k - 1, 'l');
                            break;
                        }
                        else {
                            break;
                        }
                    }
                    else if(j - k == 0){
                        move(board, i, j, k, 'l');
                    }
                }
            }
        }
    }
}

// ボードを上に動かす
void Move_U(board_2048 *board){
    for(int i = 0; i < 4; i++){
        for(int j = 1; j < 4; j++){
            if(board->grid[j][i] != 0){
                for(int k = 1; j - k >= 0; k++){
                    if(board->grid[j - k][i] != 0 &&
                       board->grid[j][i] == board->grid[j - k][i]){
                        if(board->marged_flg[j - k][i]){
                            // 合成済みのマスなら隣に移動
                            move(board, i, j, k - 1, 'u');
                            break;
                        }
                        else{
                            // 合成済みでなければそのまま合成
                            marge(board, i, j, k, 'u');
                            break;
                        }
                    }
                    else if(board->grid[j - k][i] != 0 &&
                            board->grid[j][i] != board->grid[j - k][i]){
                        if(k > 1){
                            move(board, i, j, k - 1, 'u');
                            break;
                        }
                        else{
                            break;
                        }
                    }
                    else if(j - k == 0){
                        move(board, i, j, k, 'u');
                    }
                }
            }
        }
    }
}

// ボードを下に動かす
void Move_D(board_2048 *board){
    for(int i = 0; i < 4; i++){
        for(int j = 2; j >= 0; j--){
            if(board->grid[j][i] != 0){
                for(int k = 1; j + k < 4; k++){
                    if(board->grid[j + k][i] != 0 &&
                       board->grid[j][i] == board->grid[j + k][i]){
                        if(board->marged_flg[j + k][i]){
                            // 合成済みのマスなら隣に移動
                            move(board, i, j, k - 1, 'd');
                            break;
                        }
                        else{
                            // 合成済みでなければそのまま合成
                            marge(board, i, j, k, 'd');
                            break;
                        }
                    }
                    else if(board->grid[j + k][i] != 0 &&
                            board->grid[j][i] != board->grid[j + k][i] ){
                        if(k > 1){
                            move(board, i, j, k - 1, 'd');
                            break;
                        }
                        else {
                            break;
                        }
                    }
                    else if(j + k == 3){
                        move(board, i, j, k, 'd');
                    }
                }
            }
        }
    }
}

// 新しく数字をボードに追加する
void Pop_value(board_2048 *board){
    // 空きマス分だけの一様分布の乱数を生成
    uniform_int_distribution<int> dist3(0, board->vacant_total - 1);
    int max   = dist3(mt);
    int value = dist2(mt);
    int counter = 0;
    bool temp_flg = false;
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            if(board->grid[i][j] == 0){
                if(counter == max){
                    board->grid[i][j] = value;
                    temp_flg = true;
                    break;
                }
                counter++;
            }
        }
        if(temp_flg){
            break;
        }
    }
    board->vacant_total--;
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
            // ボード表示のついでにフラグも下ろす
            board->marged_flg[i][j] = false;
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

bool canR;
bool canL;
bool canU;
bool canD;

int main(){
    board_2048 *board = new board_2048;
    init_board(board);
    print_board(board);
    while(!board->gameover_flg){
        CanMove_grid(board);
        canR = CanMove_R(board);
        canL = CanMove_L(board);
        canU = CanMove_U(board);
        canD = CanMove_D(board);
        if(canR || canL || canU || canD){
            //ユーザーから方向を取得するための変数
            string input;
            cout << "select direction(w:↑, a:←, s:↓, d:→)" << endl;
            cout << "→:" << canR << endl;
            cout << "←:" << canL << endl;
            cout << "↑:" << canU << endl;
            cout << "↓:" << canD << endl;
            getline(cin, input);
            if(input == "w"){
                if(!canU){
                    cout << "Invaild direction" << endl << endl;
                }
                else{
                    Move_U(board);
                    Pop_value(board);
                }
            }
            else if(input == "a"){
                if(!canL){
                    cout << "Invaild direction" << endl << endl;
                }
                else{
                    Move_L(board);
                    Pop_value(board);
                }
            }
            else if(input == "s"){
                if(!canD){
                    cout << "Invaild direction" << endl << endl;
                }
                else{
                    Move_D(board);
                    Pop_value(board);
                }
            }
            else if(input == "d"){
                if(!canR){
                    cout << "Invaild direction" << endl << endl;
                }
                else{
                    Move_R(board);
                    Pop_value(board);
                }
            }
            else cout << "Invaild input" << endl << endl;

            print_board(board);
        }
        else board->gameover_flg = true;
    }
    cout << "gameover" << endl;
    cout << "SCORE: " << calc_score(board);

    delete board;
    return 0;
}