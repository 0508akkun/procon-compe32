#include "search.hpp"
#include "swap.hpp"

void FindTargetCoordinate(State& state)
{
    for (int y = 0; y <= NumOfDiv::Horizontal; y++)
    {
        for (int x = 0; x <= NumOfDiv::Vertical; x++)
        {
            if (state.status[x][y] == state.targetNum)
            {
                state.targetCoordinate.x = x;
                state.targetCoordinate.y = y;
            }
        }
    }
}

void Select(State& state, Coordinate num)
{
    state.selectPiece.x = num.x;
    state.selectPiece.y = num.y;
    state.result += std::to_string(num.y) + " " + std::to_string(num.y) + '\n';
}

std::string Search(State state, board goal, std::vector<Coordinate> correct)
{
    Select(state, correct[goal[NumOfDiv::Vertical][NumOfDiv::Horizontal]]);
    for (int y = 0; y <= NumOfDiv::Horizontal; y++)
    {
        for (int x = 0; x <= NumOfDiv::Vertical; x++)
        {
            state.targetNum = state.status[x][y];
            FindTargetCoordinate(state);
            if (state.targetCoordinate.x > state.selectPiece.x)
            {
                while (state.targetCoordinate.x != state.selectPiece.x - 1)
                {
                    SwapWithR(state.status, state.selectPiece.x, state.selectPiece.y);
                    state.result += 'R';
                    FindTargetCoordinate(state);
                }
                if (state.targetCoordinate.y > state.selectPiece.y)
                {
                    while (state.targetCoordinate.y - 1 != state.selectPiece.y)
                    {
                        SwapWithU(state.status, state.selectPiece.x, state.selectPiece.y);
                        state.result += 'U';
                        FindTargetCoordinate(state);
                    }
                }
                else
                {
                    while (state.targetCoordinate.y - 1 != state.selectPiece.y)
                    {
                        SwapWithD(state.status, state.selectPiece.x, state.selectPiece.y);
                        state.result += 'D';
                        FindTargetCoordinate(state);
                    }
                }
                SwapWithR(state.status, state.selectPiece.x, state.selectPiece.y);
                state.result += 'R';
                FindTargetCoordinate(state);
            }
            else
            {
                while (state.targetCoordinate.x != state.selectPiece.x + 1)
                {
                    SwapWithL(state.status, state.selectPiece.x, state.selectPiece.y);
                    state.result += 'L';
                    FindTargetCoordinate(state);
                }
                if (state.targetCoordinate.y > state.selectPiece.y)
                {
                    while (state.targetCoordinate.y - 1 != state.selectPiece.y)
                    {
                        SwapWithU(state.status, state.selectPiece.x, state.selectPiece.y);
                        state.result += 'U';
                        FindTargetCoordinate(state);
                    }
                }
                else
                {
                    while (state.targetCoordinate.y - 1 != state.selectPiece.y)
                    {
                        SwapWithD(state.status, state.selectPiece.x, state.selectPiece.y);
                        state.result += 'D';
                        FindTargetCoordinate(state);
                    }
                }
                SwapWithL(state.status, state.selectPiece.x, state.selectPiece.y);
                state.result += 'L';
                FindTargetCoordinate(state);
            }
            while (state.targetCoordinate.y != y)
            {
                SwapWithR(state.status, state.selectPiece.x, state.selectPiece.y);
                state.result += 'R';
                FindTargetCoordinate(state);
                SwapWithU(state.status, state.selectPiece.x, state.selectPiece.y);
                state.result += 'U';
                FindTargetCoordinate(state);
                SwapWithU(state.status, state.selectPiece.x, state.selectPiece.y);
                state.result += 'U';
                FindTargetCoordinate(state);
                SwapWithL(state.status, state.selectPiece.x, state.selectPiece.y);
                state.result += 'L';
                FindTargetCoordinate(state);
                SwapWithD(state.status, state.selectPiece.x, state.selectPiece.y);
                state.result += 'D';
                FindTargetCoordinate(state);
            }
            SwapWithR(state.status, state.selectPiece.x, state.selectPiece.y);
            state.result += 'R';
            FindTargetCoordinate(state);
            SwapWithU(state.status, state.selectPiece.x, state.selectPiece.y);
            state.result += 'U';
            FindTargetCoordinate(state);
            while (state.targetCoordinate.x != x)
            {
                SwapWithD(state.status, state.selectPiece.x, state.selectPiece.y);
                state.result += 'D';
                FindTargetCoordinate(state);
                SwapWithL(state.status, state.selectPiece.x, state.selectPiece.y);
                state.result += 'L';
                FindTargetCoordinate(state);
                SwapWithL(state.status, state.selectPiece.x, state.selectPiece.y);
                state.result += 'L';
                FindTargetCoordinate(state);
                SwapWithU(state.status, state.selectPiece.x, state.selectPiece.y);
                state.result += 'U';
                FindTargetCoordinate(state);
                SwapWithR(state.status, state.selectPiece.x, state.selectPiece.y);
                state.result += 'R';
                FindTargetCoordinate(state);
            }

            for (int i = 0; i <= NumOfDiv::Horizontal; i++) {
                for (int k = 0; k <= NumOfDiv::Vertical; k++) {
                    if (state.distance[k][i] == 0)
                    {
                        std::cout << "\33[44m" << state.status[k][i] << "\33[m" << " ";
                        continue;
                    }
                    std::cout << state.status[k][i] << " ";
                }
                std::cout << std::endl;
            }
            std::cout << state.result << std::endl;
            std::cout << "-----------------------" << std::endl;
        }
    }

    return state.result;
}