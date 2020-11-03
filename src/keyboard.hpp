#ifndef KEYBOARD_HPP
#define KEYBOARD_HPP

#include "constants.hpp"
#include "structures.hpp"

#include <map>

#include <SDL2/SDL.h>

struct Keyboard {
private:
	int keyInputDelay = 0;
	std::map<SDL_Keycode, bool> keyState;
	std::map<SDL_Keycode, bool> keyPressed;
public:
	bool getKeyState(SDL_Keycode keyCode);
	bool isKey(SDL_Keycode keyCode);
	bool isKeyPressed(SDL_Keycode keyCode);
	void step();
	void update(SDL_Event& event);
};

static std::map<Direction, SDL_Keycode> keyDirection = {
	{NORTH, SDLK_KP_8},
	{NORTHEAST, SDLK_KP_9},
	{EAST, SDLK_KP_6},
	{SOUTHEAST, SDLK_KP_3},
	{SOUTH, SDLK_KP_2},
	{SOUTHWEST, SDLK_KP_1},
	{WEST, SDLK_KP_4},
	{NORTHWEST, SDLK_KP_7}
};

#endif // KEYBOARD_HPP
