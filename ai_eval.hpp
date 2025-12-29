#pragma once
#include "board.hpp"

// 評価関数ver.1
int evaluate_board_01(board_2048 &board);
// 評価関数ver.2
int evaluate_board_02(board_2048 &board);
// 評価関数ver.3
int evaluate_board_03(board_2048 &board);
// 評価関数ver.4
int evaluate_board_04(board_2048 &board);
// 評価関数ver.5
int evaluate_board_05(board_2048 &board);
// 評価関数を1,2が出現するときの両方の平均をとって、さらにどのマスに出現するかを考慮して空きマスすべてを検討する
int evaluate_allpatern(board_2048 &board);