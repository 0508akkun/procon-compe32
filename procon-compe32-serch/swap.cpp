#include "swap.hpp"

void SwapWithU(board& status, int& selectPieceX, int& selectPieceY)
{
    if (selectPieceY == 0)
    {
        std::swap(status[selectPieceX][0], status[selectPieceX][NumOfDiv::Horizontal]);
        selectPieceY = NumOfDiv::Horizontal;
    }
    else
    {
        std::swap(status[selectPieceX][selectPieceY], status[selectPieceX][selectPieceY-1]);
        selectPieceY--;
    }
}
void SwapWithD(board& status, int& selectPieceX, int& selectPieceY)
{
    if (selectPieceY == NumOfDiv::Horizontal)
    {
        std::swap(status[selectPieceX][selectPieceY], status[selectPieceX][0]);
        selectPieceY = 0;
    }
    else
    {
        std::swap(status[selectPieceX][selectPieceY], status[selectPieceX][selectPieceY+1]);
        selectPieceY++;
    }
}
void SwapWithR(board& status, int& selectPieceX, int& selectPieceY)
{
    if (selectPieceX == NumOfDiv::Vertical)
    {
        std::swap(status[selectPieceX][selectPieceY], status[0][selectPieceY]);
        selectPieceX = 0; 
    }
    else
    {
        std::swap(status[selectPieceX][selectPieceY], status[selectPieceX+1][selectPieceY]);
        selectPieceX++;
    }
}
void SwapWithL(board& status, int& selectPieceX, int& selectPieceY)
{
    if (selectPieceX == 0)
    {
        std::swap(status[0][selectPieceY], status[NumOfDiv::Vertical][selectPieceY]);
        selectPieceX = NumOfDiv::Vertical;
    }
    else
    {
        std::swap(status[selectPieceX][selectPieceY], status[selectPieceX-1][selectPieceY]);
        selectPieceX--;
    }
}
