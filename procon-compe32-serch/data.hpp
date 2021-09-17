#include <vector>

using board = std::vector<std::vector<int>>;

struct State {
    int numOfselect;
    int cost;

    board status;
    
    State(int _numOfselect, board _status)
      : numOfselect(_numOfselect),
        cost(0),
        status(_status)
    { }
};