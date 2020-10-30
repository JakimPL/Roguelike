#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <SDL2/SDL.h>
#include "constants.hpp"
#include "functions.hpp"
#include "log.hpp"

#include <fstream>
#include <vector>
#include <string>

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
	unsigned int red, green, blue;
	operator SDL_Color() const
	{
		uint8_t r = red;
		uint8_t g = green;
		uint8_t b = blue;
		SDL_Color sdlColor = {r, g, b, 255};
		return sdlColor;
	};
};

struct Tile {
	char letter;
	Color color;
	bool obstacle;
	unsigned int textID;
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

struct Area {
private:
	unsigned int textID;
	unsigned int width;
	unsigned int height;
	std::vector<std::vector<Tile>> map;
public:
	// create an area from a file
	Area(const std::string &filename);
	bool saveToFile(const std::string &filename);
	unsigned int getTextID();
	unsigned int getHeight();
	unsigned int getWidth();
	Tile getTile(unsigned int x, unsigned int y);
};

struct Character {
	unsigned int textID;
	long time;
	int reputation;

	bool saveToFile(const std::string &filename);
};

struct Item {
private:
	unsigned int textID;
	long description;
	ItemType type;
	ItemCategory category;
	ItemFlag flag;
	long price;
	int attack;
	int attack_rate;
	int damage;
	int damage_delta;
	unsigned int delay;
	int defense;
	int defense_rate;
	unsigned int required_level;
	unsigned int required_strength;
	unsigned int required_wisdom;
	unsigned int required_dexterity;
	unsigned int required_intelligence;
	std::vector<ItemEffect> effects;
	std::string resource;
public:
	bool saveToFile(const std::string &filename);
};

struct Creature {
private:
	unsigned int textID;
	char letter;
	Color color;
	Race race;
	Gender gender;
	unsigned int state;
	unsigned int level;
	unsigned long xp;
	unsigned long xpNextLevel;
	unsigned long xpValue;
	unsigned long gold;
	unsigned int hp;
	unsigned int hpMax;
	unsigned int mp;
	unsigned int mpMax;
	unsigned int acBase;
	unsigned int accuracy;
	unsigned int strength;
	unsigned int dexterity;
	unsigned int constitution;
	unsigned int intelligence;
	unsigned int wisdom;
	unsigned int resistance[(unsigned int)(Elementals::count)];
	std::vector<CreatureEffect> effects;
	std::vector<Ability> abilities;
public:
	Creature();
	Creature(const std::string &filename);
	bool saveToFile(const std::string &filename);

	unsigned int getTextID();
	Color getColor();
	char getLetter();
	int getLevel();
	int getRemainingXP();
	int getCurrentXP();
	int getCurrentHP();
	int getCurrentMP();
	int getMaxHP();
	int getMaxMP();

	unsigned int getWeaponTextID();
};

struct Position {
	unsigned int x, y;
	unsigned int direction : 3;
	void moveInDirection(int length = 1, Direction chosenDirection = COUNT);
	const Position operator +(int length) const;
};

#endif
