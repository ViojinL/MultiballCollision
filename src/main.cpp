#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_ttf.h>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "DataPacketBall.h"
#include "Wall.h"
#include "Button.h"

void renderWall(SDL_Renderer *renderer, Wall &wall)
{
    SDL_Rect rect;
    rect.x = static_cast<int>(wall.getX() - wall.getWidth() / 2.0);
    rect.y = static_cast<int>(wall.getY() - wall.getHeight() / 2.0);
    rect.w = static_cast<int>(wall.getWidth());
    rect.h = static_cast<int>(wall.getHeight());

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
}

int main(int argc, char *argv[])
{

    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    if (TTF_Init() != 0)
    {
        std::cerr << "TTF_Init Error: " << TTF_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    const int windowWidth = 900;

    const int windowHeight = 600;

    SDL_Window *window = SDL_CreateWindow("数据包碰转模拟",
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          windowWidth, windowHeight, SDL_WINDOW_SHOWN);
    if (!window)
    {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer)
    {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    TTF_Font *font = TTF_OpenFont("arial.ttf", 16);
    if (!font)
    {
        std::cerr << "TTF_OpenFont Error: " << TTF_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Color buttonColor = {200, 200, 200, 255};
    SDL_Color textColor = {0, 0, 0, 255};

    int buttonWidth = 80;
    int buttonHeight = 40;
    int padding = 5;
    int buttonX = 815;
    int buttonY = 10;

    Button add3Button(buttonX, buttonY, buttonWidth, buttonHeight, "+3 Balls", buttonColor, textColor);
    Button add10Button(buttonX, buttonY + buttonHeight + padding, buttonWidth, buttonHeight, "+10 Balls", buttonColor, textColor);

    std::vector<Button> buttons = {add3Button, add10Button};

    DataPacketBall *ball1 = new DataPacketBall("Packet 1", "Data 1", 30.0);
    ball1->setPosition(200.0, 100.0);
    ball1->setVelocity(2.0, 3.0);

    DataPacketBall *ball2 = new DataPacketBall("Packet 2", "Data 2", 40.0);
    ball2->setPosition(400.0, 150.0);
    ball2->setVelocity(-3.0, 2.0);

    DataPacketBall *ball3 = new DataPacketBall("Packet 3", "Data 3", 35.0);
    ball3->setPosition(600.0, 200.0);
    ball3->setVelocity(1.5, 2.5);

    std::vector<Ball *> ballsVec = {ball1, ball2, ball3};

    Wall leftWall(50.0, windowHeight / 2.0, 10.0, windowHeight);
    Wall rightWall(800.0, windowHeight / 2.0, 10.0, windowHeight);
    std::vector<Wall> walls = {leftWall, rightWall};

    bool quit = false;
    SDL_Event e;
    while (!quit)
    {

        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT)
            {
                int mouseX = e.button.x;
                int mouseY = e.button.y;

                for (size_t i = 0; i < buttons.size(); ++i)
                {
                    if (buttons[i].isClicked(mouseX, mouseY))
                    {
                        int count = 0;
                        if (i == 0)
                            count = 3;
                        else if (i == 1)
                            count = 10;

                        if (count > 0)
                        {
                            DataPacketBall::createBalls(ballsVec, count, static_cast<double>(windowWidth), static_cast<double>(windowHeight));
                            std::cout << "添加了 " << count << " 个新球。" << std::endl;
                        }
                    }
                }
            }
        }

        for (auto &ball : ballsVec)
        {
            ball->simulate(static_cast<double>(windowWidth), static_cast<double>(windowHeight));
        }

        for (size_t i = 0; i < ballsVec.size(); ++i)
        {
            for (size_t j = i + 1; j < ballsVec.size(); ++j)
            {
                if (ballsVec[i]->checkCollision(*ballsVec[j]))
                {
                    ballsVec[i]->handleCollision(*ballsVec[j]);
                }
            }
        }

        for (auto it = ballsVec.begin(); it != ballsVec.end();)
        {
            DataPacketBall *dpb = dynamic_cast<DataPacketBall *>(*it);
            if (dpb && dpb->isDestroyed())
            {
                delete *it;
                it = ballsVec.erase(it);
            }
            else
            {
                ++it;
            }
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        for (auto &button : buttons)
        {
            button.render(renderer, font);
        }

        for (auto &wall : walls)
        {
            renderWall(renderer, wall);
        }

        for (auto &ball : ballsVec)
        {
            ball->render(renderer);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    for (auto &ball : ballsVec)
    {
        delete ball;
    }

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
