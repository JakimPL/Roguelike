#ifndef TEXT_H
#define TEXT_H

#include <map>
#include <string>
#include <vector>

#define TEXT_CATEGORY_SIZE 1000000
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

enum class String : unsigned long {
	empty = 0,
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
	Alignment,
	FPS,
	AddedItem,
	BackpackIsFull,

	Moondale = (unsigned int)(TextCategory::Area) * TEXT_CATEGORY_SIZE + 1,

	Dead = (unsigned int)(TextCategory::Creature) * TEXT_CATEGORY_SIZE + 1,
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
	Julian,

	Fist = (unsigned int)(TextCategory::Item) * TEXT_CATEGORY_SIZE + 1,
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
	Unidentified,

	TooHighItemRequirements = (unsigned int)(TextCategory::Message) * TEXT_CATEGORY_SIZE + 1,

	Ground = (unsigned int)(TextCategory::Object) * TEXT_CATEGORY_SIZE + 1,
	Sand,
	Water,
	Wall,
	Road,
	Item,
};

typedef std::vector<String> StringList;
typedef std::pair<TextCategory, unsigned int> TextPair;

class Text
{
private:
	std::vector<std::vector<std::string>> content;
	bool convertFromTXTtoSTR(TextCategory category, const std::string& inputPath, const std::string& outputPath);
	bool loadContent(TextCategory category, const std::string& filename);

public:
	Text();
	unsigned int getContentSize(TextCategory category) const;
	const std::string text(TextCategory category, unsigned int id) const;
	const std::string operator[](String element);
	const std::string operator[](TextPair element) const;

	static TextPair makeTextPair(String element);
};

static const StringList raceNameIDs = {
	String::Undefined,
	String::Human,
	String::Elf,
	String::Gnome,
	String::Dwarf,
	String::Orc,
	String::Animal,
	String::MagicBeing,
	String::Construct,
	String::Demigod,
	String::God
};

static const StringList genderNameIDs = {
	String::Unknown,
	String::Male,
	String::Female,
	String::Nonbinary
};

static const StringList alignmentNameIDs = {
	String::Unknown,
	String::Good,
	String::Neutral,
	String::Evil
};

static const StringList stateNameIDs = {
	String::empty,
	String::Dead,
	String::Unconscious,
	String::Paralyzed,
	String::Sleeping,
	String::Mute,
	String::Frozen,
	String::Panic,
	String::Summoned
};

static const StringList typeNameIDs = {
	String::empty,
	String::Weapon,
	String::Armor,
	String::Helmet,
	String::Gloves,
	String::Cloak,
	String::Boots,
	String::Ring,
	String::Amulet,
	String::Quiver,
	String::Quick
};

static const StringList categoryNameIDs = {
	String::empty,
	String::Dagger,
	String::ShortSword,
	String::LongSword,
	String::Staff,
	String::Spear,
	String::Club,
	String::Axe,
	String::Bow,
	String::Crossbow,
	String::Sling,
	String::Clothing,
	String::LeatherArmor,
	String::ChainMail,
	String::PlateArmor,
	String::Robe,
	String::Potion,
	String::Spell
};

static const StringList flagNameIDs = {
	String::empty,
	String::Broken,
	String::Magic,
	String::Unidentified
};

#endif
