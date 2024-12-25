#include "DataPacketBall.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <SDL_ttf.h>
#include <cmath>
#include <random>
#include <typeinfo>

using namespace std;

map<int, SDL_Texture *> DataPacketBall::textureCache_;

SDL_Texture *DataPacketBall::getTexture(SDL_Renderer *renderer, int radius)
{

  if (textureCache_.find(radius) != textureCache_.end())
  {
    return textureCache_[radius];
  }

  int diameter = 2 * radius;
  SDL_Surface *surface = SDL_CreateRGBSurfaceWithFormat(0, diameter, diameter, 32, SDL_PIXELFORMAT_RGBA32);
  if (!surface)
  {
    cerr << "Failed to create surface: " << SDL_GetError() << endl;
    return nullptr;
  }

  SDL_FillRect(surface, nullptr, SDL_MapRGBA(surface->format, 0, 0, 0, 0));

  SDL_LockSurface(surface);
  Uint32 *pixels = (Uint32 *)surface->pixels;
  Uint32 color = SDL_MapRGBA(surface->format, 0, 0, 255, 255);

  for (int w = 0; w < diameter; ++w)
  {
    for (int h = 0; h < diameter; ++h)
    {
      int dx = radius - w;
      int dy = radius - h;
      if ((dx * dx + dy * dy) <= radius * radius)
      {
        pixels[h * diameter + w] = color;
      }
    }
  }
  SDL_UnlockSurface(surface);

  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
  if (!texture)
  {
    cerr << "Failed to create surface: " << SDL_GetError() << endl;
  }
  SDL_FreeSurface(surface);

  textureCache_[radius] = texture;
  return texture;
}

DataPacketBall::DataPacketBall(const string &name, const string &data, double size) : Ball(name, size), data_(data), shouldBeDestroyed_(false), screenWidth_(0.0) {}

std::string DataPacketBall::getData() const
{
  return data_;
}

void DataPacketBall::simulate(double screenWidth, double screenHeight)
{
  screenWidth_ = screenWidth;
  double leftWallX = 50.0;
  double rightWallX = 800.0;

  handleWallCollision(leftWallX, rightWallX);

  posX_ += velX_;
  posY_ += velY_;
  if (posY_ + size_ / 2.0 >= screenHeight)
  {
    posY_ = screenHeight - size_ / 2.0;
    velY_ = 0.0;
    velX_ = 0.0;
    if (!hasReachedBottom_)
    {
      cout << this->getName() << "已通过信道，准备销毁。" << endl;
      hasReachedBottom_ = true;

      destroyTask_ = async(std::launch::async, &DataPacketBall::destroy, this);
    }
  }
}

void DataPacketBall::handleCollision(Ball &other)
{

  double otherPosX, otherPosY;
  other.getPosition(otherPosX, otherPosY);

  double dx = otherPosX - posX_;
  double dy = otherPosY - posY_;
  double distanceSquared = dx * dx + dy * dy;

  if (distanceSquared == 0.0)
    return;

  double otherVelX, otherVelY;
  other.getVelocity(otherVelX, otherVelY);

  double dvx = otherVelX - velX_;
  double dvy = otherVelY - velY_;

  double TouyingVel = (dvx * dx + dvy * dy) / distanceSquared;

  cout << "检测到" << getName() << "和" << other.getName() << "发生了碰撞。" << endl;

  /*DataPacketBall* dpb = nullptr;
  if(typeid(other) == typeid(DataPacketBall)){
    dpb =static_cast<DataPacketBall*>(&other);
  }

  if(dpb){

  }else{
    cout << "Ball类向下转型失败"  << endl;
  }*/
  DataPacketBall *dpb = dynamic_cast<DataPacketBall *>(&other);
  if (dpb && !shouldBeDestroyed_.load() && !dpb->isDestroyed())
  {
    if (size_ < dpb->getSize())
    {
      cout << this->getData() << "优先级比" << dpb->getData() << "低，被重传" << endl;

      if (retransmitTask_.valid())
      {
        retransmitTask_.wait();
      }
      this->retransmitTask_ = async(std::launch::async, &DataPacketBall::retransmit, this, screenWidth_);
    }
    else
    {
      cout << dpb->getData() << "优先级比" << this->getData() << "低，被重传" << endl;

      if (retransmitTask_.valid())
      {
        retransmitTask_.wait();
      }
      dpb->retransmitTask_ = async(std::launch::async, &DataPacketBall::retransmit, dpb, dpb->screenWidth_);
    }
  }

  double distance = std::sqrt(distanceSquared);
  double overlap = (size_ + other.getSize()) / 2.0 - distance;
  if (overlap > 0 && distance != 0)
  {
    double moveX = (dx / distance) * (overlap / 2.0);
    double moveY = (dy / distance) * (overlap / 2.0);

    posX_ -= moveX;
    posY_ -= moveY;

    other.setPosition(otherPosX + moveX, otherPosY + moveY);
  }
}

void DataPacketBall::retransmit(double screenWidth)
{

  std::this_thread::sleep_for(std::chrono::milliseconds(500));

  std::uniform_real_distribution<double> distX(50.0 + size_ / 2.0, screenWidth - 50.0 - size_ / 2.0);
  posX_ = distX(rng_);

  posY_ = -size_ / 2.0;

  std::uniform_real_distribution<double> distVelX(-2.0, 2.0);
  std::uniform_real_distribution<double> distVelY(1.0, 3.0);
  velX_ = distVelX(rng_);
  velY_ = distVelY(rng_);

  hasReachedBottom_ = false;

  cout << this->getName() << " 被重传，新的位置为 (" << posX_ << ", " << posY_
       << ")，新速度为 (" << velX_ << ", " << velY_ << ")。" << endl;
}

void DataPacketBall::destroy()
{
  std::this_thread::sleep_for(std::chrono::seconds(3));

  shouldBeDestroyed_.store(true);

  cout << this->getName() << "已被销毁" << endl;
}

void DataPacketBall::render(SDL_Renderer *renderer)
{
  int radius = static_cast<int>(size_ / 2.0);
  SDL_Texture *texture = getTexture(renderer, radius);
  if (!texture)
    return;

  SDL_Rect dstRect;
  dstRect.x = static_cast<int>(posX_ - radius);
  dstRect.y = static_cast<int>(posY_ - radius);
  dstRect.w = radius * 2;
  dstRect.h = radius * 2;

  SDL_RenderCopy(renderer, texture, nullptr, &dstRect);
}

void DataPacketBall::createBalls(std::vector<Ball *> &balls, int count, double screenWidth, double screenHeight)
{
  for (int i = 0; i < count; ++i)
  {
    std::string name = "Packet " + std::to_string(balls.size() + 1);
    std::string data = "Data " + std::to_string(balls.size() + 1);
    double size = 30.0 + (std::rand() % 21);

    DataPacketBall *newBall = new DataPacketBall(name, data, size);

    std::uniform_real_distribution<double> distX(50.0 + size / 2.0, screenWidth - 50.0 - size / 2.0);
    double posX = distX(rng_);
    double posY = -size / 2.0;

    newBall->setPosition(posX, posY);

    std::uniform_real_distribution<double> distVelX(-2.0, 2.0);
    std::uniform_real_distribution<double> distVelY(1.0, 3.0);
    double velX = distVelX(rng_);
    double velY = distVelY(rng_);
    newBall->setVelocity(velX, velY);

    balls.push_back(newBall);
  }
}
