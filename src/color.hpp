#ifndef COLOR_HPP
#define COLOR_HPP

#include <SDL2/SDL.h>

struct Color {
	uint8_t red, green, blue;
	operator SDL_Color() const
	{
		uint8_t r = red;
		uint8_t g = green;
		uint8_t b = blue;
		SDL_Color sdlColor = {r, g, b, 255};
		return sdlColor;
	};
};

#endif // COLOR_HPP
