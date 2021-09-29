#include "eval.hpp"

int Eval(State state)
{   
    int score = 0;
    for (int y = 0; y <= NumOfDiv::Horizontal; y++)
    {
        for (int x = 0; x <= NumOfDiv::Vertical; x++)
        {
            score += state.distance[x][y];
        }
    }
    if (state.numOfselect > 0)
    {
        score += state.distance[state.selectPieceX][state.selectPieceY];
    }
    score += state.cost;
    score -= 5 * state.numOfselect;
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