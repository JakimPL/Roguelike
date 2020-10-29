#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include "constants.hpp"
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

namespace Functions
{

bool compareHeaders(const char* header1, const char* header2);
void drawLetter(SDL_Renderer* renderer, TTF_Font* font, const char letter, const SDL_Color color, const unsigned short x, const unsigned short y);
void drawRectangle(SDL_Renderer* renderer, const unsigned short x, const unsigned short y, const unsigned short w, const unsigned short h, const SDL_Color sdlColor);
void drawText(SDL_Renderer* renderer, TTF_Font* font, const std::string text, const SDL_Color color, const unsigned short x, const unsigned short y, const Alignment hAlign = Alignment::Left, const Alignment vAlign = Alignment::Top);
unsigned int getValue(const char* chars);

};

#endif // FUNCTIONS_HPP
