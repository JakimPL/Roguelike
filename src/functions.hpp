#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

namespace Functions
{

bool compareHeaders(const char *header1, const char *header2);
void drawLetter(SDL_Renderer* renderer, TTF_Font* font, const char letter, const SDL_Color color, const unsigned short x, const unsigned short y);
void drawRectangle(SDL_Renderer* renderer, unsigned short x, unsigned short y, unsigned short w, unsigned short h, SDL_Color sdlColor);

};

#endif // FUNCTIONS_HPP
