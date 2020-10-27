#ifndef STRUCTURES_H
#define STRUCTURES_H

#include "constants.hpp"
#include "functions.hpp"
#include "log.hpp"

#include <fstream>
#include <vector>
#include <string>

enum class Allegiance {
	unknown,
	pc,
	ally,
	charmed,
	neutral,
	enemy,
	count
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
};

struct ItemEffect {
	unsigned int name;
};

struct CreatureEffect {
	unsigned int name;
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
	unsigned int name;
};

struct Area {
	unsigned int name;
	unsigned int width;
	unsigned int height;
	std::vector<std::vector<char>> backgroudMap;
	std::vector<std::vector<Color>> colorMap;

	// create an area from a file
	Area(const std::string &filename);
	bool saveToFile(const std::string &filename);
};

struct Character {
	unsigned int name;
	long time;
	int reputation;

	bool saveToFile(const std::string &filename);
};

struct Item {
	unsigned int name;
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

	bool saveToFile(const std::string &filename);
};

struct Creature {
	unsigned int name;
	Color color;
	char letter;
	Race race;
	Gender gender;
	unsigned int state;
	unsigned int level;
	unsigned long xp_current;
	unsigned long xp_next_level;
	unsigned long xp_value;
	unsigned long gold;
	unsigned int hp;
	unsigned int hp_max;
	unsigned int mp;
	unsigned int mp_max;
	unsigned int ac_natural;
	unsigned int accuracy;
	unsigned int strength;
	unsigned int dexterity;
	unsigned int constitution;
	unsigned int intelligence;
	unsigned int wisdom;
	unsigned int resistance[(unsigned int)(Elementals::count)];
	std::vector<CreatureEffect> effects;
	std::vector<Ability> abilities;

	Creature();
	Creature(const std::string &filename);
	bool saveToFile(const std::string &filename);
};

struct Position {
	unsigned long x, y;
	unsigned int direction : 3;
};

enum class String {
	empty,
	Roguelike,
	HP,
	MP,
	InvChar,
	NextLevel,
	Next,
	XP,
	X,
	Y,
	STR,
	DEX,
	CON,
	INT,
	WIS,
	Gold,
	Att,
	Acc,
	level,
	Moondale
};

#endif
