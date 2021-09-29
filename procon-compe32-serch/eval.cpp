#include "eval.hpp"

int Eval(State state)
{   
    int score = 0;
    for (int y = 0; y <= NumOfDiv::Horizontal; y++)
    {
        for (int x = 0; x <= NumOfDiv::Vertical; x++)
        {
            score += 20 * state.distance[x][y];
        }
    }
    score += state.cost;
    score -= 5 * state.numOfselect;
    for (int i = 0; i < state.result.size()-1; i++)
    {
        if (state.result.substr(i, 2) == "RL" || state.result.substr(i, 2) == "LR" || state.result.substr(i, 2) == "UD" || state.result.substr(i, 2) == "DU")
        {
            score += 10;
        }
    }
    return score;
}

void FindDistance(State& state, const std::vector<Coordinate> correctCoordinate)
{
    for (int y = 0; y <= NumOfDiv::Horizontal; y++)
    {
        for (int x = 0; x <= NumOfDiv::Vertical; x++)
        {
            int correctX = correctCoordinate[state.status[x][y]].x;
            int correctY = correctCoordinate[state.status[x][y]].y;
            state.distance[x][y] = abs(x - correctX) + abs(y - correctY);
        }
    }
}

void FindCorrectCoordinate(std::vector<Coordinate>& correctCoordinate, const board& goal)
{
    for (int y = 0; y <= NumOfDiv::Horizontal; y++)
    {
        for (int x = 0; x <= NumOfDiv::Vertical; x++)
        {
            correctCoordinate[goal[x][y]].x = x;
            correctCoordinate[goal[x][y]].y = y;
        }
    }
}