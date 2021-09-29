#include <iostream>
#include <stdio.h>
#include <vector>
#include "data.hpp"
#include "search.hpp"
#include "eval.hpp"

int NumOfDiv::Vertical;
int NumOfDiv::Horizontal;

int main()
{
    int numOfselect;
    int numOfDivV;
    int numOfDivH;
    int selectCostRate;
    int swapCostRate;

    std::cin >> numOfDivV >> numOfDivH;
    std::cin >> numOfselect;
    std::cin >> selectCostRate >> swapCostRate;

    std::vector<std::vector<int>> status(numOfDivV, std::vector<int>(numOfDivH));
    std::vector<std::vector<int>> init(numOfDivV, std::vector<int>(numOfDivH));
    std::vector<Coordinate> correctCoordiante(numOfDivH*numOfDivV);

    for (int i = 0; i < numOfDivH; i++) {
        for (int k = 0; k < numOfDivV; k++) {
            std::cin >> status[k][i];
            init[k][i] = k + i * numOfDivH;
        }
    }

    std::cout << "====================" << std::endl;

    NumOfDiv::Vertical = numOfDivV - 1;
    NumOfDiv::Horizontal = numOfDivH - 1;

    State initialState(numOfselect, init);

    for (int i = 0; i <= NumOfDiv::Horizontal; i++) {
        for (int k = 0; k <= NumOfDiv::Vertical; k++) {
            std::cout << status[k][i] << " ";
        }
        std::cout << "\n";
    }
    std::cout << "---------------------------------------" << std::endl;

    FindCorrectCoordinate(correctCoordiante, status);

    BeamSearch(initialState, correctCoordiante, selectCostRate, swapCostRate);

}