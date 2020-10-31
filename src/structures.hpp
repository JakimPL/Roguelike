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

enum Direction {
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

enum class Race {
	undefined,
	human,
	elf,
	gnome,
	dwarf,
	orc,
	animal,
	monster,
	magic_being,
	demigod,
	god,
	count
};

enum class State {
	dead,
	sleeping,
	mute,
	frozen,
	panic,
	summoned,
	undead,
	count
};

enum class Elementals {
	physical,
	magic,
	fire,
	ice,
	electricity,
	acid,
	missile,
	count
};

enum class Gender {
	unknown,
	male,
	female,
	nonbinary,
	count
};

enum class CreatureCharacter {
	unknown,
	good,
	neutral,
	evil,
	count
};

enum class Stack {
	unused,
	weapon,
	armor,
	helmet,
	gloves,
	cloak,
	boots,
	ring,
	amulet,
	quiver,
	quick,
	count
};

enum class ItemType {
	miscellaneous,
	weapon,
	armor,
	helmet,
	gloves,
	cloak,
	boots,
	ring,
	amulet,
	quiver,
	quick,
	count
};

enum class ItemCategory {
	unused,
	long_sword,
	short_sword,
	staff,
	spear,
	club,
	axe,
	bow,
	crossbow,
	small_arms,
	sling,
	leather_armor,
	chain_mall,
	plate_armor,
	robe,
	count
};

enum class ItemFlag {
	none,
	broken,
	magic,
	count
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
	const Position operator +(int length) const;
};

struct ItemEffect {
	unsigned int textID;
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

#endif
