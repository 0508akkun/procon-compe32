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
    std::string fileName = "solution.txt";
    std::ofstream writingFile;
    writingFile.open(fileName, std::ios::out);

    writingFile << rotate << std::endl;
    writingFile << std::to_string(selectCoordinate.size()) << std::endl;
    for (int i = 0; i < selectCoordinate.size(); i++)
    {
        writingFile << std::hex << selectCoordinate[i].x << selectCoordinate[i].y << std::dec << std::endl;
        writingFile << move[i].size() << std::endl;
        writingFile << move[i] << std::endl;
    }

    writingFile.close();
}