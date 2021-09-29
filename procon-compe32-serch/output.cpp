#include <iostream>
#include <fstream>
#include "output.hpp"

void Output(std::string result, std::string rotate)
{
    std::vector<std::string> line;
    std::vector<Coordinate> selectCoordinate;
    std::vector<std::string> move;
    int prev = 0;
    for (int i = 0; i < result.size(); i++)
    {
        if (result[i] == '\n')
        {
            line.push_back(result.substr(prev, i - prev));
            prev = i+1;
        }
    }
    for (auto i : line) std::cout << i << std::endl;
    for (int i = 0; i < line.size(); i += 2)
    {
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
        move.push_back(line[i+1]);
    }

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