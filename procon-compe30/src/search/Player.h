# pragma once
# include <iostream>
# include <vector>
# include "Point.h"
struct Player
{
    Point pos;
    Player();
    Player(Point);
    Player(int,int);
};