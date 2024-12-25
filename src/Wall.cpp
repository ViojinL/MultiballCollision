#include "Wall.h"

Wall::Wall(double x, double y, double width, double height)
    : x_(x), y_(y), width_(width), height_(height) {}

double Wall::getX() const
{
    return x_;
}

double Wall::getY() const
{
    return y_;
}

double Wall::getWidth() const
{
    return width_;
}

double Wall::getHeight() const
{
    return height_;
}
