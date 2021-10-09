#pragma once
#include <vector>
#include <string>

using board = std::vector<std::vector<int>>;

struct State {
    int numOfselect;
    int cost;

    int selectPieceX;
    int selectPieceY;

    int count;

    std::string result;

    board status;
    board distance;
    
    State(int _numOfselect, board _status)
      : numOfselect(_numOfselect),
        cost(0),
        selectPieceX(0),
        selectPieceY(0),
        count(0),
        status(_status),
        distance(_status)
    { }
};

struct Coordinate {
    int x;
    int y;

    Coordinate()
    { }

    Coordinate(int _x, int _y)
      : x(_x),
        y(_y)
    { }
};

struct NumOfDiv {
    static int Vertical;
    static int Horizontal;
};