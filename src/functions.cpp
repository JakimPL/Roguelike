#include "functions.hpp"
#include "constants.hpp"
#include "structures.hpp"

#include <cstring>

namespace Functions
{

bool compareHeaders(const char *header1, const char *header2)
{
	return (std::strcmp(header1, header2) == 0);
}

void drawLetter(SDL_Renderer* renderer, TTF_Font* font, const char letter, const SDL_Color color, const unsigned short x, const unsigned short y)
{
	const char text[] = {letter};
	SDL_Surface* textSurface = TTF_RenderText_Solid(font, text, color);
	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
	SDL_Rect textRectangle;
	textRectangle.x = TILE_WIDTH * x;
	textRectangle.y = TILE_HEIGHT * y;
	textRectangle.w = TILE_WIDTH;
	textRectangle.h = TILE_HEIGHT;
	SDL_RenderCopy(renderer, textTexture, NULL, &textRectangle);
	SDL_DestroyTexture(textTexture);
}

void drawRectangle(SDL_Renderer* renderer, unsigned short x, unsigned short y, unsigned short w, unsigned short h, SDL_Color sdlColor)
{
	SDL_Rect upperRectangle;
	upperRectangle.x = x;
	upperRectangle.y = y;
	upperRectangle.w = w;
	upperRectangle.h = h;
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, sdlColor.r, sdlColor.g, sdlColor.b, sdlColor.a);
	SDL_RenderFillRect(renderer, &upperRectangle);
}

}
