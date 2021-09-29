#include "search.hpp"

const int BeamWidth = 1024;

bool Check(State state)
{
    for (int i = 0; i <= NumOfDiv::Horizontal; i++)
    {
        for (int k = 0; k <= NumOfDiv::Vertical; k++)
        {
            if(state.distance[k][i] != 0) return false;
        }
    }
    return true;
}

bool Check2(State state)
{
    if (state.distance[state.selectPieceX][state.selectPieceY] == 0) return true;
    else return false;
}

bool Comp(std::pair<int, State> lhs, std::pair<int, State> rhs)
{
    return lhs.first < rhs.first;
}

std::string BeamSearch(State initialState, std::vector<Coordinate> correctCoordinate, int selectCostRate, int swapCostRate)
{
    int itr = 0;

    std::deque<std::pair<int, State>> beam;
    std::deque<std::pair<int, State>> nexts;

    Select(initialState);

    std::pair<int, State> state(Eval(initialState), initialState);

    beam.push_front(state);

    while (!beam.empty())
    {
        itr++;
        state = beam.front();
        beam.pop_front();
        if (Check(state.second)) break;
        if (Check2(state.second) && state.second.numOfselect > 0) 
        {
            ChangeSelection(state.second);
            state.second.cost += selectCostRate;
        }

        std::pair<int, State> newState(state);

        SwapWithU(newState.second.status, newState.second.selectPieceX, newState.second.selectPieceY);
        FindDistance(newState.second, correctCoordinate);
        newState.second.cost += swapCostRate;
        newState.second.result += 'U';
        newState.first = Eval(newState.second);
        nexts.push_back(newState);
        newState = state;

        SwapWithD(newState.second.status, newState.second.selectPieceX, newState.second.selectPieceY);
        FindDistance(newState.second, correctCoordinate);
        newState.second.cost += swapCostRate;
        newState.second.result += 'D';
        newState.first = Eval(newState.second);
        nexts.push_back(newState);
        newState = state;

        SwapWithR(newState.second.status, newState.second.selectPieceX, newState.second.selectPieceY);
        FindDistance(newState.second, correctCoordinate);
        newState.second.cost += swapCostRate;
        newState.second.result += 'R';
        newState.first = Eval(newState.second);
        nexts.push_back(newState);
        newState = state;

        SwapWithL(newState.second.status, newState.second.selectPieceX, newState.second.selectPieceY);
        FindDistance(newState.second, correctCoordinate);
        newState.second.cost += swapCostRate;
        newState.second.result += 'L';
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

        if (itr == 1000)
        {
            std::cout << "Select: " << state.second.status[state.second.selectPieceX][state.second.selectPieceY];
            std::cout << " Score: " << state.first << std::endl;
            for (int i = 0; i <= NumOfDiv::Horizontal; i++) {
                for (int k = 0; k <= NumOfDiv::Vertical; k++) {
                    std::cout << state.second.status[k][i] << "(" << state.second.distance[k][i] << ")" << " ";
                }
                std::cout << std::endl;
            }
            std::cout << state.second.result << std::endl;
            std::cout << "-----------------------" << std::endl;
            itr = 0;
        }
    }

    if (Check(state.second))
    {
        for (int i = 0; i <= NumOfDiv::Horizontal; i++) {
            for (int k = 0; k <= NumOfDiv::Vertical; k++) {
                std::cout << state.second.status[k][i] << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
        std::cout << state.second.result << std::endl;
        std::cout << std::endl;
    }
    else
    {
        std::cout << "Not found" << std::endl;
    }
    state.second.result += '\n';
    return state.second.result;
}

void Select(State& state)
{
    int maxDist = 0;
    int maxX = 0;
    int maxY = 0;
    for (int i = 0; i <= NumOfDiv::Horizontal; i++) 
    {
        for (int k = 0; k <= NumOfDiv::Vertical; k++) 
        {
            if (state.distance[k][i] > maxDist)
            {
                maxDist = state.distance[k][i];
                maxX = k;
                maxY = i;
            }
        }
    }
    state.selectPieceX = maxX;
    state.selectPieceY = maxY;
    state.result += std::to_string(maxX) + " " + std::to_string(maxY) + '\n';
}

void ChangeSelection(State& state)
{  
    int maxDist = 0;
    int maxX = 0;
    int maxY = 0;
    for (int i = 0; i <= NumOfDiv::Horizontal; i++) 
    {
        for (int k = 0; k <= NumOfDiv::Vertical; k++) 
        {
            if (state.distance[k][i] > maxDist)
            {
                maxDist = state.distance[k][i];
                maxX = k;
                maxY = i;
            }
        }
    }
    state.selectPieceX = maxX;
    state.selectPieceY = maxY;
    state.numOfselect--;
    state.result += '\n' + std::to_string(maxX) + " " + std::to_string(maxY) + '\n';
}