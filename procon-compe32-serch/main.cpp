#include <iostream>
#include <vector>

#include "data.hpp"

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

    std::vector<std::vector<int>> status(numOfDivH, std::vector<int>(numOfDivV));

    for (int i = 0; i < numOfDivH; i++) {
        for (int k = 0; k < numOfDivV; k++) {
            std::cin >> status[i][k];
        }
    }

    State state(numOfselect, status);

    BeamSearch(state, selectCostRate, swapCostRate);

    for (auto a : state.status) {
        for(auto b : a) {
            std::cout << b << " ";
        }
        std::cout << std::endl;
    }
}