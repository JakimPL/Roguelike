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
	Item,
	Object,
	Count
};

static const std::string FILENAME_STRING[(unsigned int)(TextCategory::Count)] = {
	"GENERAL",
	"AREA",
	"ITEM",
	"OBJECT",
};

namespace String
{

enum class General : unsigned int {
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
	WeaponInUse,
	Damage,
	Defense,
	AttackRate,
	DefenseRate,
	Delay,
	Speed,
	RequiredLevel
};

enum class Area : unsigned int {
	empty,
	Moondale
};

enum class Item : unsigned int {
	empty,
	Fist,
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

enum class Object : unsigned int {
	empty,
	Ground,
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
	const std::string operator[](String::Item element);
	const std::string operator[](String::Object element);
	const std::string operator[](std::pair<TextCategory, unsigned int> element);

	unsigned int getContentSize(TextCategory category);
};

static std::vector<Text*> TextCounter;

#endif
