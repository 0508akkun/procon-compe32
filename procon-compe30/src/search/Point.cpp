# include "Point.h"

const Point Point::Left{ 0, -1 };
const Point Point::Right{ 0, 1 };
const Point Point::Up{ -1, 0 };
const Point Point::Down{ 1, 0 };

Point::Point(int _y, int _x)
    : y(_y)
    , x(_x)
{
}

Point::Point(const Point& point)
    : y(point.y)
    , x(point.x)
{
}

