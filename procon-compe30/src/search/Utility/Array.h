# pragma once
# include <iostream>
# include <vector>

/// <summary>
/// 一次元配列を標準出力します。
/// </summary>
template <class Type>
void DisplayArray(const std::vector<Type>& ary)
{
    std::cout << "{ ";
    for (const auto& a : ary)
    {
        std::cout << a << " ";
    }
    std::cout << "}\n";
}
/// <summary>
/// 二次元配列を標準出力します。
/// </summary>
template <class Type>
void DisplayArray(const std::vector<std::vector<Type>>& ary)
{
    for (int i = 0; i < ary.size(); ++i)
    {
        std::cout << "{ ";
        for (int k = 0; k < ary[i].size(); ++k)
        {
            std::cout << ary[i][k] << " ";
        }
        std::cout << "}\n";
    }
}