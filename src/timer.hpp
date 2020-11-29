#ifndef TIMER_HPP
#define TIMER_HPP

#include "options.hpp"

#include <SDL2/SDL.h>

struct Timer {
private:
	float fps;
	Uint32 start = 0;
	Uint32 end = 0;
	Uint32 delta = 0;
public:
	bool frame();
	void update();
	float getFPS() const;
};

static Timer timer;

#endif // TIMER_HPP
