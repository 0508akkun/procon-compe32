#include "Eval.hpp"

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