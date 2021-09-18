#pragma once
#include <iostream>
#include <algorithm>
#include "data.hpp"

void SwapWithU(board& status, int& selectPieceX, int& selectPieceY);
void SwapWithD(board& status, int& selectPieceX, int& selectPieceY);
void SwapWithR(board& status, int& selectPieceX, int& selectPieceY);
void SwapWithL(board& status, int& selectPieceX, int& selectPieceY);
void Swap(int& a, int& b);