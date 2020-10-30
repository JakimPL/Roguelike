#include "functions.hpp"
#include "constants.hpp"
#include "structures.hpp"

#include <cstring>

namespace Functions
{

bool compareHeaders(const char* header1, const char* header2)
{
	return (std::strcmp(header1, header2) == 0);
}

void drawLetter(SDL_Renderer* renderer, TTF_Font* font, const char letter, const SDL_Color color, const unsigned short x, const unsigned short y)
{
	const char text[] = {letter};
	SDL_Surface* textSurface = TTF_RenderText_Blended(font, text, color);
	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
	SDL_Rect textRectangle;
	textRectangle.x = _TILE_WIDTH * x;
	textRectangle.y = _TILE_HEIGHT * y;
	textRectangle.w = _TILE_WIDTH;
	textRectangle.h = _TILE_HEIGHT;
	SDL_RenderCopy(renderer, textTexture, NULL, &textRectangle);
	SDL_FreeSurface(textSurface);
	SDL_DestroyTexture(textTexture);
}

void drawRectangle(SDL_Renderer* renderer, unsigned short x, unsigned short y, unsigned short w, unsigned short h, SDL_Color sdlColor)
{
	SDL_Rect upperRectangle;
	upperRectangle.x = x / SCALE;
	upperRectangle.y = y / SCALE;
	upperRectangle.w = w / SCALE;
	upperRectangle.h = h / SCALE;
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, sdlColor.r, sdlColor.g, sdlColor.b, sdlColor.a);
	SDL_RenderFillRect(renderer, &upperRectangle);
}

void drawText(SDL_Renderer* renderer, TTF_Font* font, const std::string text, const SDL_Color color, const unsigned short x, const unsigned short y, const Alignment hAlign, const Alignment vAlign)
{
	int _x = x / SCALE;
	int _y = y / SCALE;

	std::stringstream textStream;
	textStream << text;
	const char* textChar = text.c_str();

	int textWidth, textHeight;
	TTF_SizeUTF8(font, textChar, &textWidth, &textHeight);
	SDL_Surface* textSurface = TTF_RenderUTF8_Blended(font, textChar, color);
	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
	SDL_Rect textRectangle;
	textRectangle.x = _x - textWidth * hAlign / 2;
	textRectangle.y = _y - textHeight * vAlign / 2;
	textRectangle.w = textWidth;
	textRectangle.h = textHeight;

	SDL_RenderCopy(renderer, textTexture, NULL, &textRectangle);
	SDL_FreeSurface(textSurface);
	SDL_DestroyTexture(textTexture);
}

unsigned int getValue(const char* chars)
{
	unsigned int value = 0;
	for (size_t i = 0; i < SIZE_INT; ++i) {
		value += (unsigned int)(chars[i]) << (8 * i);
	}

	return value;
}

}
