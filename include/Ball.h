#pragma once

#include <string>
#include <vector>
#include <random>
#include <SDL.h>

using namespace std;

class Ball
{
public:
    Ball(const string &name, double size);
    virtual ~Ball() = default;

    void setPosition(double x, double y);
    void getPosition(double &x, double &y) const;

    void setVelocity(double velX, double velY);
    void getVelocity(double &velX, double &velY) const;

    double getSize() const;
    string getName() const;

    virtual void simulate(double screenWidth, double screenHeight) = 0;

    bool checkCollision(const Ball &other) const;

    virtual void handleCollision(Ball &other) = 0;

    virtual void render(SDL_Renderer *renderer) = 0;

protected:
    void handleWallCollision(double leftWallX, double rightWallX);

    std::string name_;
    double size_;
    double posX_, posY_;
    double velX_, velY_;

    bool hasReachedBottom_;

    static std::mt19937 rng_;
};