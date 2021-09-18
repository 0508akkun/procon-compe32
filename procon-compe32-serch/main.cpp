#include <iostream>
#include <stdio.h>
#include <vector>
#include "data.hpp"
#include "search.hpp"

int NumOfDiv::Vertical;
int NumOfDiv::Horizontal;

int main()
{
    int numOfselect;
    int numOfDivV;
    int numOfDivH;
    int selectCostRate;
    int swapCostRate;

    std::cin >> numOfselect;
    std::cin >> numOfDivV >> numOfDivH;
    std::cin >> selectCostRate >> swapCostRate;

    std::vector<std::vector<int>> status(numOfDivV, std::vector<int>(numOfDivH));

    for (int i = 0; i < numOfDivH; i++) {
        for (int k = 0; k < numOfDivV; k++) {
            std::cin >> status[k][i];
        }
    }

    std::cout << "====================" << std::endl;


    State state(numOfselect, status);
    NumOfDiv::Vertical = numOfDivV - 1;
    NumOfDiv::Horizontal = numOfDivH - 1;

    for (int i = 0; i <= NumOfDiv::Horizontal; i++) {
        for (int k = 0; k <= NumOfDiv::Vertical; k++) {
            std::cout << status[k][i] << " ";
        }
        std::cout << "\n";
    }
    std::cout << "---------------------------------------" << std::endl;
    BeamSearch(state, selectCostRate, swapCostRate);

}