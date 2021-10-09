#pragma once
#include <cmath>
#include <deque>
#include <utility>
#include "data.hpp"
#include "swap.hpp"
#include "eval2.hpp"

bool Check(State state);

bool Check2(int target);

bool Comp(std::pair<int, State> lhs, std::pair<int, State> rhs);

std::string BeamSearch2(State initialState, board goal, int selectCostRate, int swapCostRate);

void Select(State& state);

void NextTarget();