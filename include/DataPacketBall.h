#pragma once

#include "Ball.h"
#include <atomic>
#include <future>
#include <map>

using namespace std;

class DataPacketBall : public Ball
{
public:
    DataPacketBall(const string &name, const string &data, double size);
    ~DataPacketBall() = default;

    std::string getData() const;

    void simulate(double screenWidth, double screenHeight) override;
    void handleCollision(Ball &other) override;
    void render(SDL_Renderer *renderer) override;

    static void createBalls(std::vector<Ball *> &balls, int count, double screenWidth, double screenHeight);

    bool isDestroyed() const { return shouldBeDestroyed_.load(); }

private:
    void retransmit(double screenWidth);
    void destroy();

    std::string data_;

    std::future<void> retransmitTask_;
    std::future<void> destroyTask_;

    static std::map<int, SDL_Texture *> textureCache_;
    static SDL_Texture *getTexture(SDL_Renderer *renderer, int radius);

    std::atomic<bool> shouldBeDestroyed_; // 并发，原子操作

    double screenWidth_;
};
