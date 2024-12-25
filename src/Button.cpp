#include "Button.h"
#include <SDL_ttf.h>
#include <iostream>

using namespace std;

Button::Button(int x, int y, int width, int height, const string &label, SDL_Color color, SDL_Color textColor)
    : label_(label), color_(color), textColor_(textColor)
{
    rect_.x = x;
    rect_.y = y;
    rect_.w = width;
    rect_.h = height;
}

Button::~Button() {}

void Button::render(SDL_Renderer *renderer, TTF_Font *font)
{

    SDL_SetRenderDrawColor(renderer, color_.r, color_.g, color_.b, color_.a);
    SDL_RenderFillRect(renderer, &rect_);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &rect_);

    SDL_Surface *textSurface = TTF_RenderText_Blended(font, label_.c_str(), textColor_);
    if (!textSurface)
    {
        std::cerr << "TTF_RenderText_Blended Error: " << TTF_GetError() << std::endl;
        return;
    }

    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (!textTexture)
    {
        std::cerr << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
        SDL_FreeSurface(textSurface);
        return;
    }

    int textW = 0, textH = 0;
    SDL_QueryTexture(textTexture, NULL, NULL, &textW, &textH);
    SDL_Rect textRect;
    textRect.x = rect_.x + (rect_.w - textW) / 2;
    textRect.y = rect_.y + (rect_.h - textH) / 2;
    textRect.w = textW;
    textRect.h = textH;

    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}

bool Button::isClicked(int mouseX, int mouseY) const
{
    return (mouseX >= rect_.x && mouseX <= rect_.x + rect_.w &&
            mouseY >= rect_.y && mouseY <= rect_.y + rect_.h);
}
