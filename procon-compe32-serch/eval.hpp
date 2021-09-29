#pragma once
#include <cmath>
#include <tuple>
#include "data.hpp"

int Eval(State state);

void FindDistance(State& state, const std::vector<Coordinate> correctCoordinate);

void FindCorrectCoordinate(std::vector<Coordinate>& correctCoordinate, const board& goal);