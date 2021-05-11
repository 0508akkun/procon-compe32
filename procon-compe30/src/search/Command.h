# pragma once
# include <vector>
# include "Point.h"
# include "Act.h"

struct Command
{
    Act a;
    Point to;
    Command();
    Command(Act,Point);
};

struct Commands
{
    std::vector<Command> cmds;
    Commands();
    Commands(int);
};