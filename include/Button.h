#pragma once

#include <SDL.h>
#include <string>
#include <SDL_ttf.h>
using namespace std;
class Button
{
public:
    Button(int x, int y, int width, int height, const string &label, SDL_Color color, SDL_Color textColor);
    ~Button();

    void render(SDL_Renderer *renderer, TTF_Font *font);

    bool isClicked(int mouseX, int mouseY) const;

private:
    SDL_Rect rect_;
    std::string label_;
    SDL_Color color_;
    SDL_Color textColor_;
};