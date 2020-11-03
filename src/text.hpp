#ifndef TEXT_H
#define TEXT_H

#include <string>
#include <vector>

#define STRING(value) std::to_string(value)

#include "constants.hpp"

enum class String : unsigned int {
	empty,
	Roguelike,
	HP,
	MP,
	SHORTCUTS,
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
	EmptyBackpack,
	Dmg,
	Acc,
	level,
	Moondale,
	WeaponInUse,
	Damage,
	Defense,
	AttackRate,
	DefenseRate,
	Delay,
	Speed,
	Dagger,
	ShortSword,
	LongSword,
	Staff,
	Spear,
	Club,
	Axe,
	Bow,
	Crossbow,
	Sling,
	Clothing,
	LeatherArmor,
	ChainMail,
	PlateArmor,
	Robe,
	Weapon,
	Armor,
	Helmet,
	Gloves,
	Cloak,
	Boots,
	Ring,
	Amulet,
	Quiver,
	Quick,
	Potion,
	Spell,
	Fist,
	Ground,
	Sand,
	Water
};

class Text
{
private:
	std::vector<std::string> content;
	bool convertFromTXTtoSTR(const std::string& inputPath, const std::string& outputPath);
	bool loadContent(const std::string& filename);

public:
	Text();
	const std::string text(unsigned int id) const;
	const std::string operator[](String id) const;
	const std::string operator[](unsigned int id) const;
};

static std::vector<Text*> TextCounter;

#endif
