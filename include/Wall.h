#pragma once
class Wall
{
public:
    Wall(double x, double y, double width, double height);
    ~Wall() = default;

    double getX() const;
    double getY() const;
    double getWidth() const;
    double getHeight() const;

private:
    double x_;
    double y_;
    double width_;
    double height_;
};