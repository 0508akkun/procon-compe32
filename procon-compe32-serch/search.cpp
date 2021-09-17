#include <deque>
#include <utility>

#include "data.hpp"

bool check(board state)
{
    for (int i = 0; i < state.size(); i++)
    {
        for (int k = 0; k < state[0].size(); k++)
        {
            if(state[i][k] != i * k) return false;
        }
    }
    return true;
}

int Eval(State state)
{
    return 1;
}

void BeamSearch(State initialState, int selectCostRate, int swapCostRate)
{
    std::deque<std::pair<int, State>> beam;
    std::deque<std::pair<int, State>> nexts;

    std::pair<int, State> state = std::make_pair(Eval(initialState), initialState);

    beam.push_front(state);

    while (!beam.empty())
    {
        state = beam.front();
        beam.pop_front();
        if (check(state.second.status)) break;
    }
}