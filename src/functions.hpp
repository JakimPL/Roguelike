#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include "constants.hpp"
#include "structures.hpp"
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

namespace Functions
{

bool compareHeaders(const char* header1, const char* header2);
//SDL_Color createColor(Color color, uint8_t alpha);
void drawLetter(SDL_Renderer* renderer, TTF_Font* font, const char letter, const SDL_Color color, const unsigned short x, const unsigned short y);
void drawRectangle(SDL_Renderer* renderer, const SDL_Color color, const unsigned short x, const unsigned short y, const unsigned short w, const unsigned short h, const bool outline = false);
void drawText(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, const SDL_Color color, const unsigned short x, const unsigned short y, const Alignment hAlign = Alignment::Left, const Alignment vAlign = Alignment::Center);
const std::string getPath(const std::string& filename, Filetype filetype);
void read(std::ifstream& resource, char* string, unsigned int size);

};

#endif // FUNCTIONS_HPP
