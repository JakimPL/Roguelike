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
	Dialog,
	Item,
	Message,
	Object,
	Count
};

static const std::string FILENAME_STRING[(unsigned int)(TextCategory::Count)] = {
	"GENERAL",
	"AREA",
	"CREATURE",
	"DIALOG",
	"ITEM",
	"MESSAGE",
	"OBJECT",
};

enum String : unsigned long {
	s_empty = 0,
	s_Roguelike,
	s_HP,
	s_MP,
	s_SHORTCUTS,
	s_NextLevel,
	s_Next,
	s_XP,
	s_X,
	s_Y,
	s_STR,
	s_DEX,
	s_CON,
	s_INT,
	s_WIS,
	s_Gold,
	s_EmptyBackpack,
	s_Def,
	s_DefRate,
	s_AttMin,
	s_AttMax,
	s_Acc,
	s_level,
	s_WeaponInUse,
	s_Damage,
	s_Defense,
	s_AttackRate,
	s_DefenseRate,
	s_Delay,
	s_Speed,
	s_RequiredLevel,
	s_Experience,
	s_HealthPoints,
	s_ManaPoints,
	s_HealthRegeneration,
	s_ManaRegeneration,
	s_Strength,
	s_Dexterity,
	s_Constitution,
	s_Intelligence,
	s_Wisdom,
	s_Alignment,
	s_FPS,
	s_AddedItem,
	s_BackpackIsFull,
	s_Closed,
	s_NotEnoughGold,
	s_SoldItem,

	s_Moondale = (unsigned int)(TextCategory::Area) * TEXT_CATEGORY_SIZE + 1,

	s_Dead = (unsigned int)(TextCategory::Creature) * TEXT_CATEGORY_SIZE + 1,
	s_Unconscious,
	s_Paralyzed,
	s_Sleeping,
	s_Mute,
	s_Frozen,
	s_Panic,
	s_Summoned,
	s_Undefined,
	s_Human,
	s_Elf,
	s_Gnome,
	s_Dwarf,
	s_Orc,
	s_Animal,
	s_MagicBeing,
	s_Construct,
	s_Demigod,
	s_God,
	s_Unknown,
	s_Male,
	s_Female,
	s_Nonbinary,
	s_Good,
	s_Neutral,
	s_Evil,
	s_Julian,

	s_Fist = (unsigned int)(TextCategory::Item) * TEXT_CATEGORY_SIZE + 1,
	s_Dagger,
	s_ShortSword,
	s_LongSword,
	s_Staff,
	s_Spear,
	s_Club,
	s_Axe,
	s_Bow,
	s_Crossbow,
	s_Sling,
	s_Clothing,
	s_LeatherArmor,
	s_ChainMail,
	s_PlateArmor,
	s_Robe,
	s_Weapon,
	s_Armor,
	s_Helmet,
	s_Gloves,
	s_Cloak,
	s_Boots,
	s_Ring,
	s_Amulet,
	s_Quiver,
	s_Quick,
	s_Potion,
	s_Spell,
	s_Broken,
	s_Magic,
	s_Unidentified,

	s_TooHighItemRequirements = (unsigned int)(TextCategory::Message) * TEXT_CATEGORY_SIZE + 1,

	s_Ground = (unsigned int)(TextCategory::Object) * TEXT_CATEGORY_SIZE + 1,
	s_Sand,
	s_Water,
	s_Wall,
	s_Road,
	s_Item,
	s_Door,
	s_Bridge,
	s_Floor,
	s_Sign
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
	const std::string operator[](String element) const;
	const std::string operator[](TextPair element) const;

	static TextPair makeTextPair(String element);
};

static const StringList raceNameIDs = {
	s_Undefined,
	s_Human,
	s_Elf,
	s_Gnome,
	s_Dwarf,
	s_Orc,
	s_Animal,
	s_MagicBeing,
	s_Construct,
	s_Demigod,
	s_God
};

static const StringList genderNameIDs = {
	s_Unknown,
	s_Male,
	s_Female,
	s_Nonbinary
};

static const StringList alignmentNameIDs = {
	s_Unknown,
	s_Good,
	s_Neutral,
	s_Evil
};

static const StringList stateNameIDs = {
	s_empty,
	s_Dead,
	s_Unconscious,
	s_Paralyzed,
	s_Sleeping,
	s_Mute,
	s_Frozen,
	s_Panic,
	s_Summoned
};

static const StringList typeNameIDs = {
	s_empty,
	s_Weapon,
	s_Armor,
	s_Helmet,
	s_Gloves,
	s_Cloak,
	s_Boots,
	s_Ring,
	s_Amulet,
	s_Quiver,
	s_Quick
};

static const StringList categoryNameIDs = {
	s_empty,
	s_Dagger,
	s_ShortSword,
	s_LongSword,
	s_Staff,
	s_Spear,
	s_Club,
	s_Axe,
	s_Bow,
	s_Crossbow,
	s_Sling,
	s_Clothing,
	s_LeatherArmor,
	s_ChainMail,
	s_PlateArmor,
	s_Robe,
	s_Potion,
	s_Spell
};

static const StringList flagNameIDs = {
	s_empty,
	s_Broken,
	s_Magic,
	s_Unidentified
};

#endif
