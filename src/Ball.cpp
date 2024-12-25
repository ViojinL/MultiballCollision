#include "Ball.h"
#include <cmath>
#include <iostream>

using namespace std;

mt19937 Ball::rng_(random_device{}());

Ball::Ball(const string &name, double size)
    : name_(name), size_(size), posX_(0), posY_(0), velX_(0), velY_(0), hasReachedBottom_(false) {}

void Ball::setPosition(double x, double y)
{
    posX_ = x;
    posY_ = y;
}

void Ball::getPosition(double &x, double &y) const
{
    x = posX_;
    y = posY_;
}

void Ball::setVelocity(double velX, double velY)
{
    velX_ = velX;
    velY_ = velY;
}

void Ball::getVelocity(double &velX, double &velY) const
{
    velX = velX_;
    velY = velY_;
}

double Ball::getSize() const
{
    return size_;
}

std::string Ball::getName() const
{
    return name_;
}

bool Ball::checkCollision(const Ball &other) const
{
    double dx = posX_ - other.posX_;
    double dy = posY_ - other.posY_;
    double distance = sqrt(dx * dx + dy * dy);
    return distance < (size_ + other.size_) / 2.0;
}

void Ball::handleWallCollision(double leftWallX, double rightWallX)
{
    if (posX_ - size_ / 2.0 <= leftWallX)
    {
        velX_ = abs(velX_);
    }
    else if (posX_ + size_ / 2.0 >= rightWallX)
    {
        velX_ = -abs(velX_);
    }
}
