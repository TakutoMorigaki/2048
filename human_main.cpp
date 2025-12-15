#include <iostream>
#include <iomanip>
#include "board.hpp"
using namespace std;

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

    
    char temp;
    cout << "press Enter key to quit..." << endl;
    cin >> temp;

    delete board;
    return 0;
}