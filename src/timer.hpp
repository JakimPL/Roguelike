#ifndef TIMER_HPP
#define TIMER_HPP

#include "constants.hpp"
#include <SDL2/SDL.h>

struct Timer {
private:
	float fps = FPS;
	Uint32 start = 0;
	Uint32 end = 0;
	Uint32 delta = 0;
public:
	bool frame();
	void update();
};

#endif // TIMER_HPP
