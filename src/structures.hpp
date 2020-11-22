#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <fstream>
#include <map>
#include <string>
#include <vector>

#define DIR_X(dir) (dir % 4 > 0 ? (dir / 4 > 0 ? -1 : 1) : 0)
#define DIR_Y(dir) ((dir + 2) % 4 > 0 ? ((((dir + 2) % 8) / 4 > 0) ? 1 : -1) : 0)

enum Ability : size_t {
	strength,
	dexterity,
	constitution,
	intelligence,
	wisdom,
	count
};

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

struct Position {
	int x, y;
	unsigned int direction : 3;
	void moveInDirection(int length = 1, Direction chosenDirection = COUNT);
	const Position operator +(int length) const;
	const Position operator -(int length) const;
	const Position operator +(Position position) const;
	const Position operator -(Position position) const;

	Position(int _x = 0, int _y = 0, int dir = 4);
};

struct CreatureEffect {
	unsigned int nameID;
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

struct Character {
	unsigned int nameID;
	long time;
	int reputation;

	bool saveToFile(const std::string& filename);
};

#endif
