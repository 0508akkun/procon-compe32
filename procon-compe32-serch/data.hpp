#pragma once
#include <vector>

using board = std::vector<std::vector<int>>;

struct State {
    int numOfselect;
    int cost;

    int selectPieceX;
    int selectPieceY;

    std::vector<char> result;

    board status;
    
    State(int _numOfselect, board _status)
      : numOfselect(_numOfselect),
        cost(0),
        selectPieceX(0),
        selectPieceY(1),
        status(_status)
    { }
};

struct NumOfDiv {
    static int Vertical;
    static int Horizontal;
};