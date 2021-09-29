#pragma once
#include <cmath>
#include <deque>
#include <utility>
#include "data.hpp"
#include "swap.hpp"
#include "eval.hpp"

bool Check(State state);

bool Check2(State state);

bool Comp(std::pair<int, State> lhs, std::pair<int, State> rhs);

std::string BeamSearch(State initialState, std::vector<Coordinate> correctCoordinate, int selectCostRate, int swapCostRate);

void Select(State& state);

void ChangeSelection(State& state);