#include <iostream>
#include <stdio.h>
#include <vector>
#include "data.hpp"
#include "output.hpp"
#include "search.hpp"
#include "Eval.hpp"

int NumOfDiv::Vertical;
int NumOfDiv::Horizontal;

int main()
{
    //選択可能数、分割数、コストレートの変数
    int numOfselect;
    int numOfDivV;
    int numOfDivH;
    int selectCostRate;
    int swapCostRate;

    std::cin >> numOfDivV >> numOfDivH;
    std::cin >> numOfselect;
    std::cin >> selectCostRate >> swapCostRate;

    //正しい位置を格納する配列と初期状態を格納する配列
    std::vector<std::vector<int>> status(numOfDivV, std::vector<int>(numOfDivH));
    std::vector<std::vector<int>> init(numOfDivV, std::vector<int>(numOfDivH));
    //正しい座標を格納する配列
    std::vector<Coordinate> correctCoordiante(numOfDivH*numOfDivV);
    //回転情報
    std::string rotate = "";

    for (int i = 0; i < numOfDivH; i++) {
        for (int k = 0; k < numOfDivV; k++) {
            std::cin >> status[k][i];
            init[k][i] = k + i * numOfDivV;
        }
    }
    for (int i = 0; i < numOfDivH; i++) {
        for (int k = 0; k < numOfDivV; k++) {
            char a;
            std::cin >> a;
            rotate.push_back(a);
        }
    }

    std::cout << "====================" << std::endl;

    //分割数を記録する変数
    NumOfDiv::Vertical = numOfDivV - 1;
    NumOfDiv::Horizontal = numOfDivH - 1;

    State initialState(numOfselect, init);

    for (int i = 0; i <= NumOfDiv::Horizontal; i++) {
        for (int k = 0; k <= NumOfDiv::Vertical; k++) {
            std::cout << status[k][i] << " ";
        }
        std::cout << "\n";
    }
    for (int i = 0; i <= NumOfDiv::Horizontal; i++) {
        for (int k = 0; k <= NumOfDiv::Vertical; k++) {
            std::cout << init[k][i] << " ";
        }
        std::cout << "\n";
    }
    std::cout << rotate << std::endl;
    std::cout << "---------------------------------------" << std::endl;

    FindCorrectCoordinate(correctCoordiante, status);

    std::string result = Search(initialState, status, correctCoordiante);

    Output(result, rotate);
}