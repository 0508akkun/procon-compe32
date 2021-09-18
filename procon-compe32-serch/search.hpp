#pragma once
#include <deque>
#include <utility>
#include "data.hpp"
#include "swap.hpp"

bool Chack(board state);

int Eval(State state);

bool Comp(std::pair<int, State> lhs, std::pair<int, State> rhs);

bool Find(std::vector<board> vec, board a);

void BeamSearch(State initialState, int selectCostRate, int swapCostRate);
