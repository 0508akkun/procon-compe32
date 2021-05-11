# pragma once
# include <string>

enum class Act:int
{
    Stay,
    Move,
    Remove,
};

std::string ActToString(const Act a);