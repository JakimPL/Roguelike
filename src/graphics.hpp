#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include "constants.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

namespace Graphics
{

//SDL_Color createColor(Color color, uint8_t alpha);
void drawLetter(SDL_Renderer* renderer, TTF_Font* font, const char letter, const SDL_Color color, const unsigned short x, const unsigned short y);
void drawPixel(SDL_Renderer* renderer, const SDL_Color color, const unsigned short x, const unsigned short y, const unsigned short size);
void drawRectangle(SDL_Renderer* renderer, const SDL_Color color, const unsigned short x, const unsigned short y, const unsigned short w, const unsigned short h, const bool outline = false);
void drawText(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, const SDL_Color color, const unsigned short x, const unsigned short y, const Alignment hAlign = Alignment::Left, const Alignment vAlign = Alignment::Center);

}

#endif // GRAPHICS_HPP
