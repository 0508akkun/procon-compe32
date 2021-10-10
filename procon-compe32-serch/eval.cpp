#include "eval.hpp"

//評価関数(スコアが小さい方が良い)
int Eval(State state)
{   
    int score = 0;
    for (int y = 0; y <= NumOfDiv::Horizontal; y++)
    {
        for (int x = 0; x <= NumOfDiv::Vertical; x++)
        {
            if (state.distance[x][y] == 0)
            {
                score -= 50;
            }
            score += 130 * state.distance[x][y];
        }
    }
    score += state.cost;
    //同じ状態に戻る動作は不要
    for (int i = 0; i < state.result.size()-1; i++)
    {
        if (state.result.substr(i, 2) == "RL" || state.result.substr(i, 2) == "LR" || state.result.substr(i, 2) == "UD" || state.result.substr(i, 2) == "DU")
        {
            score += 1000;
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