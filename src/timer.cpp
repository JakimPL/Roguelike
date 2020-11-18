#include "timer.hpp"

bool Timer::frame()
{
	if (delta >= 1000.0f / FPS) {
		fps = 1000.0f / delta;
		end = start;
		return true;
	}

	return false;
}

void Timer::update()
{
	start = SDL_GetTicks();
	delta = start - end;
}

float Timer::getFPS() const
{
	return fps;
}
