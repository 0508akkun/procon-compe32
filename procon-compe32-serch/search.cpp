#include "search.hpp"

const int BeamWidth = 16;

bool Check(board state)
{
    for (int i = 0; i <= NumOfDiv::Horizontal; i++)
    {
        for (int k = 0; k <= NumOfDiv::Vertical; k++)
        {
            if(state[k][i] != (NumOfDiv::Vertical+1)*i+(k+1)) return false;
        }
    }
    return true;
}

int Eval(State state)
{   
    int score = 0;
    for (int i = 0; i < NumOfDiv::Horizontal; i++)
    {
        for (int k = 0; k < NumOfDiv::Vertical; k++)
        {
            if(state.status[k][i] == (NumOfDiv::Vertical+1)*i+(k+1)) score++;
        }
    }
    return score;
}

bool Comp(std::pair<int, State> lhs, std::pair<int, State> rhs)
{
    return lhs.first > rhs.first;
}

bool Find(std::vector<board> vec, board a)
{
    auto itr = std::find(vec.begin(), vec.end(), a);
    if (itr == vec.end())
    {
        return false;
    }
    else 
    {
        return true;
    }
}

void BeamSearch(State initialState, int selectCostRate, int swapCostRate)
{
    std::deque<std::pair<int, State>> beam;
    std::deque<std::pair<int, State>> nexts;

    std::pair<int, State> state(Eval(initialState), initialState);

    beam.push_front(state);

    while (!beam.empty())
    {
        std::cout << '\n' << beam.size() << std::endl;
        std::cout << nexts.size() << std::endl;
        state = beam.front();
        beam.pop_front();
        if (Check(state.second.status)) break;

        std::pair<int, State> newState(state);

        SwapWithU(newState.second.status, newState.second.selectPieceX, newState.second.selectPieceY);
        newState.second.cost += swapCostRate;
        newState.second.result.push_back('U');
        std::cout << "U ";
        newState.first = Eval(newState.second);
        nexts.push_back(newState);
        newState = state;

        SwapWithD(newState.second.status, newState.second.selectPieceX, newState.second.selectPieceY);
        newState.second.cost += swapCostRate;
        newState.second.result.push_back('D');
        std::cout << "D ";
        newState.first = Eval(newState.second);
        nexts.push_back(newState);
        newState = state;

        SwapWithR(newState.second.status, newState.second.selectPieceX, newState.second.selectPieceY);
        newState.second.cost += swapCostRate;
        newState.second.result.push_back('R');
        std::cout << "R ";
        newState.first = Eval(newState.second);
        nexts.push_back(newState);
        newState = state;

        SwapWithL(newState.second.status, newState.second.selectPieceX, newState.second.selectPieceY);
        newState.second.cost += swapCostRate;
        newState.second.result.push_back('L');
        std::cout << "L" << std::endl;
        newState.first = Eval(newState.second);
        nexts.push_back(newState);
        newState = state;

        if (beam.empty())
        {
            std::sort(nexts.begin(), nexts.end(), Comp);
            for (int i = 0; i < BeamWidth; i++)
            {
                beam.push_back(nexts.front());
                nexts.pop_front();
                if (nexts.empty()) break;
            }
            nexts.clear();
        }
    }

    if (Check(state.second.status))
    {
        for (int i = 0; i <= NumOfDiv::Horizontal; i++) {
            for (int k = 0; k <= NumOfDiv::Vertical; k++) {
                std::cout << state.second.status[k][i] << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
        for (auto i : state.second.result) std::cout << i;
        std::cout << std::endl;
    }
    else
    {
        std::cout << "Not found" << std::endl;
    }
}