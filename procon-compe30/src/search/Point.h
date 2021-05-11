# pragma once
# include <cmath>

struct Point
{
    Point() = default;
    Point(int _y, int _x);
    Point(const Point& point);

    int y, x;

    bool isOver(int h, int w) const {
        return x < 0 || w-1 < x || y < 0 || h-1 < y;
    }
    

    double length() const {
        return std::sqrt(lengthSq());
    }

    int lengthSq() const {
        return this->dot(*this);
    }

    double lengthInv() const {
        return (double) 1.0 / length();
    }

    int distanceFrom(const Point& v) const {
        return (*this - v).length();
    }

    int distanceFromSq(const Point& v) const {
        return (*this - v).lengthSq();
    }

    int manhattanDistanceFrom(const Point& v) const {
        return std::abs(x - v.x) + std::abs(y - v.y);
    }

    // マンハッタン距離の別名
    int L1DistanceFrom(const Point& v) const {
        return std::abs(x - v.x) + std::abs(y - v.y);
    }

    int dot(const Point& other) const {
        return x*other.x + y*other.y;
    }

    bool isZero() const {
        return x == 0 && y == 0;
    }

    Point movedBy(int dy, int dx) const { return { y + dy, x + dx }; }
    Point movedBy(const Point& dr) const { return { y + dr.y, x + dr.x }; }

    Point moveBy(int dy, int dx) { y += dy; x += dx; return *this; }
    Point moveBy(const Point& dr) { y += dr.y; x += dr.x; return *this; }

    Point operator +() const { return *this; }
    Point operator -() const { return {-y, -x}; }

    Point operator +(const Point& other) const { return {y + other.y, x + other.x}; }
    Point operator -(const Point& other) const { return {y - other.y, x + other.x}; }
    Point operator *(int k) const { return {k*y, k*x}; }
    Point operator /(int k) const { return {y/k, x/k}; }

    Point& operator +=(const Point& other) {
        y += other.y;
        x += other.x;
        return *this;
    }

    Point& operator -=(const Point& other) {
        y -= other.y;
        x -= other.x;
        return *this;
    }

    Point& operator *=(int k) {
        y *= k;
        x *= k;
        return *this;
    }

    Point& operator /=(int k) {
        y /= k;
        x /= k;
        return *this;
    }

    bool operator ==(const Point& other) const { 
        return (y == other.y && x == other.x);
    }

    bool operator !=(const Point& other) const { 
        return !(y == other.y && x == other.x);
    }

    bool operator <(const Point& other) const {
        return x < other.x && y < other.y;
    }

    bool operator >(const Point& other) const {
        return lengthSq() > other.lengthSq();
    }

    bool operator <=(const Point& other) const {
        return lengthSq() <= other.lengthSq();
    }

    bool operator >=(const Point& other) const {
        return lengthSq() >= other.lengthSq();
    }

    /// <summary>
    /// Point{ 0, -1 }
    /// </summary>
    const static Point Left;

    /// <summary>
    /// Point{ 0, 1 }
    /// </summary>
    const static Point Right;

    /// <summary>
    /// Point{ -1, 0 }
    /// </summary>
    const static Point Up;

    /// <summary>
    /// Point{ 1, 0 }
    /// </summary>
    const static Point Down;
};

