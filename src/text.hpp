#ifndef TEXT_H
#define TEXT_H

#include <map>
#include <string>
#include <vector>

#define STRING(value) std::to_string(value)

#include "constants.hpp"

enum class TextCategory : unsigned int {
	General,
	Area,
	Creature,
	Item,
	Message,
	Object,
	Count
};

static const std::string FILENAME_STRING[(unsigned int)(TextCategory::Count)] = {
	"GENERAL",
	"AREA",
	"CREATURE",
	"ITEM",
	"MESSAGE",
	"OBJECT",
};

namespace String
{

enum General : unsigned int {
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
	Def,
	DefRate,
	AttMin,
	AttMax,
	Acc,
	level,
	WeaponInUse,
	Damage,
	Defense,
	AttackRate,
	DefenseRate,
	Delay,
	Speed,
	RequiredLevel,
	Experience,
	HealthPoints,
	ManaPoints,
	HealthRegeneration,
	ManaRegeneration,
	Strength,
	Dexterity,
	Constitution,
	Intelligence,
	Wisdom,
	Alignment
};

enum Area : unsigned int {
	Moondale = 1
};

enum Creature : unsigned int {
	Dead = 1,
	Unconscious,
	Paralyzed,
	Sleeping,
	Mute,
	Frozen,
	Panic,
	Summoned,
	Undefined,
	Human,
	Elf,
	Gnome,
	Dwarf,
	Orc,
	Animal,
	MagicBeing,
	Construct,
	Demigod,
	God,
	Unknown,
	Male,
	Female,
	Nonbinary,
	Good,
	Neutral,
	Evil,
	Julian
};

enum Item : unsigned int {
	Fist = 1,
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
	Broken,
	Magic,
	Unidentified
};

enum Message : unsigned int {
	TooHighItemRequirements = 1
};

enum Object : unsigned int {
	Ground = 1,
	Sand,
	Water
};

}

class Text
{
private:
	std::map<TextCategory, std::vector<std::string>> content;
	bool convertFromTXTtoSTR(TextCategory category, const std::string& inputPath, const std::string& outputPath);
	bool loadContent(TextCategory category, const std::string& filename);

public:
	Text();
	///TODO: more abstract version
	const std::string text(TextCategory category, unsigned int id);

	const std::string operator[](String::General element);
	const std::string operator[](String::Area element);
	const std::string operator[](String::Creature element);
	const std::string operator[](String::Item element);
	const std::string operator[](String::Message element);
	const std::string operator[](String::Object element);
	const std::string operator[](std::pair<TextCategory, unsigned int> element);

	unsigned int getContentSize(TextCategory category);
};

static const std::pair<TextCategory, std::vector<unsigned int>> raceNameIDs = {TextCategory::Creature, {
		String::Creature::Undefined,
		String::Creature::Human,
		String::Creature::Elf,
		String::Creature::Gnome,
		String::Creature::Dwarf,
		String::Creature::Orc,
		String::Creature::Animal,
		String::Creature::MagicBeing,
		String::Creature::Construct,
		String::Creature::Demigod,
		String::Creature::God
	}
};

static const std::pair<TextCategory, std::vector<unsigned int>> genderNameIDs = {TextCategory::Creature, {
		String::Creature::Unknown,
		String::Creature::Male,
		String::Creature::Female,
		String::Creature::Nonbinary
	}
};

static const std::pair<TextCategory, std::vector<unsigned int>> alignmentNameIDs = {TextCategory::Creature, {
		String::Creature::Unknown,
		String::Creature::Good,
		String::Creature::Neutral,
		String::Creature::Evil
	}
};

static const std::pair<TextCategory, std::vector<unsigned int>> stateNameIDs = {TextCategory::Creature, {
		String::General::empty,
		String::Creature::Dead,
		String::Creature::Unconscious,
		String::Creature::Paralyzed,
		String::Creature::Sleeping,
		String::Creature::Mute,
		String::Creature::Frozen,
		String::Creature::Panic,
		String::Creature::Summoned
	}
};

static const std::pair<TextCategory, std::vector<unsigned int>> typeNameIDs = {TextCategory::Item, {
		String::General::empty,
		String::Item::Weapon,
		String::Item::Armor,
		String::Item::Helmet,
		String::Item::Gloves,
		String::Item::Cloak,
		String::Item::Boots,
		String::Item::Ring,
		String::Item::Amulet,
		String::Item::Quiver,
		String::Item::Quick
	}
};

static const std::pair<TextCategory, std::vector<unsigned int>> categoryNameIDs = {TextCategory::Item, {
		String::General::empty,
		String::Item::Dagger,
		String::Item::ShortSword,
		String::Item::LongSword,
		String::Item::Staff,
		String::Item::Spear,
		String::Item::Club,
		String::Item::Axe,
		String::Item::Bow,
		String::Item::Crossbow,
		String::Item::Sling,
		String::Item::Clothing,
		String::Item::LeatherArmor,
		String::Item::ChainMail,
		String::Item::PlateArmor,
		String::Item::Robe,
		String::Item::Potion,
		String::Item::Spell
	}
};

static const std::pair<TextCategory, std::vector<unsigned int>> flagNameIDs = {TextCategory::Item, {
		String::General::empty,
		String::Item::Broken,
		String::Item::Magic,
		String::Item::Unidentified
	}
};

#endif
