#ifndef COLOR_HPP
#define COLOR_HPP

#include <SDL2/SDL.h>

#include <QColor>

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
	operator QColor() const
	{
		uint8_t r = red;
		uint8_t g = green;
		uint8_t b = blue;
		QColor qColor = {r, g, b, 255};
		return qColor;
	};
	bool operator==(Color rhs) const
	{
		return red == rhs.red and
			   green == rhs.green and
			   blue == rhs.blue;
	};
};

#endif // COLOR_HPP
