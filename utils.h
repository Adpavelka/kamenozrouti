#ifndef UTILS_H
#define UTILS_H

#include "board.h"

int getScore(int n);
long long heuristicEstimate(const Board &b);
Board randomBoard(int seed = -1);
Board initialBoard();
void printBoard(const Board &b);

#endif
