#pragma once
#include <vector>
#include <string>

using board = std::vector<std::vector<int>>;

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

struct State {
    int numOfselect;
    int cost;

    int count;

    int targetNum;

    std::string result;

    Coordinate targetCoordinate;
    Coordinate target;
    Coordinate selectPiece;

    board status;
    board distance;
    
    State(int _numOfselect, board _status)
      : numOfselect(_numOfselect),
        cost(0),
        count(0),
        targetCoordinate(0, 0),
        target(0, 0),
        selectPiece(0, 0),
        status(_status),
        distance(_status)
    { }
};

struct NumOfDiv {
    static int Vertical;
    static int Horizontal;
};