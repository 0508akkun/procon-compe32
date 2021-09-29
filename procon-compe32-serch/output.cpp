#include <iostream>
#include <fstream>
#include "output.hpp"

void Output(std::string result, std::string rotate)
{

    std::vector<std::string> line;              //行ごとに取り出す
    std::vector<Coordinate> selectCoordinate;   //選択ピースの座標
    std::vector<std::string> move;              //交換操作
    int prev = 0;

    //改行で分割する
    for (int i = 0; i < result.size(); i++)
    {
        if (result[i] == '\n')
        {
            line.push_back(result.substr(prev, i - prev));
            prev = i+1;
        }
    }
    for (auto i : line) std::cout << i << std::endl;
    //2行ずづ取り出していく
    for (int i = 0; i < line.size(); i += 2)
    {
        //選択したピースの座標
        Coordinate coordinate;
        std::string str = line[i];
        for (int k = 0; k < str.size(); k++)
        {
            if (line[i][k] == ' ')
            {
                coordinate.x = atoi(str.substr(0, k).c_str());
                coordinate.y = atoi(str.substr(k).c_str());
                selectCoordinate.push_back(coordinate);
                break;
            }
        }
        //交換操作
        move.push_back(line[i+1]);
    }

    //ファイルに出力
    std::string output = "";
    output += rotate + '\n';
    output += std::to_string(selectCoordinate.size()) + '\n';
    for (int i = 0; i < selectCoordinate.size(); i++)
    {
        output += std::to_string(selectCoordinate[i].x) + std::to_string(selectCoordinate[i].y) + '\n';
        output += std::to_string(move[i].size()) + '\n';
        output += move[i] + '\n';
    }

    std::ofstream writingFile;
    std::string fileName = "solution.txt";
    writingFile.open(fileName, std::ios::out);
    writingFile << output;
    writingFile.close();
}