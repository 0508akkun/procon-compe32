# include "Player.h"

Player::Player()
      : pos(0, 0)
{
}

Player::Player(Point p)
      : pos(p)
{
}

Player::Player(int _y,int _x)
      : pos(_y,_x)
{
}