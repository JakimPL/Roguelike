#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <SDL2/SDL.h>
#include "constants.hpp"
#include "functions.hpp"
#include "log.hpp"

#include <fstream>
#include <map>
#include <string>
#include <vector>

#define DIR_X(dir) (dir % 4 > 0 ? (dir / 4 > 0 ? -1 : 1) : 0)
#define DIR_Y(dir) ((dir + 2) % 4 > 0 ? ((((dir + 2) % 8) / 4 > 0) ? 1 : -1) : 0)

enum class Allegiance {
	unknown,
	pc,
	ally,
	charmed,
	neutral,
	enemy,
	count
};

enum Direction : unsigned int {
	NORTH,
	NORTHEAST,
	EAST,
	SOUTHEAST,
	SOUTH,
	SOUTHWEST,
	WEST,
	NORTHWEST,
	COUNT
};

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

struct Position {
	int x, y;
	unsigned int direction : 3;
	void moveInDirection(int length = 1, Direction chosenDirection = COUNT);
	void setDirection(Direction newDirection);
	const Position operator +(int length) const;
};

struct CreatureEffect {
	unsigned int textID;
};

enum class Target {
	none,
	preset,
	self,
	party,
	all_including_party,
	all_excluding_party,
	count
};

struct Ability {
	unsigned int textID;
};

struct Character {
	unsigned int textID;
	long time;
	int reputation;

	bool saveToFile(const std::string& filename);
};

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

#endif
