#include "timer.hpp"

bool Timer::frame()
{
	if (delta > 1000 / fps) {
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
