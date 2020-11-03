#ifndef ITEM_HPP
#define ITEM_HPP

#include "structures.hpp"
#include "color.hpp"
#include "text.hpp"

enum class Elementals : unsigned char {
	physical,
	magic,
	fire,
	ice,
	electricity,
	acid,
	missile,
	count
};

enum class Stack : unsigned char {
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

enum class ItemType : unsigned char {
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

enum class ItemCategory : unsigned char {
	unused,
	dagger,
	short_sword,
	long_sword,
	staff,
	spear,
	club,
	axe,
	bow,
	crossbow,
	sling,
	clothing,
	leather_armor,
	chain_mall,
	plate_armor,
	robe,
	potion,
	spell,
	count
};

enum class ItemFlag {
	none,
	broken,
	magic,
	count
};

struct ItemEffect {
	unsigned int textID;
};

struct Item {
public:
	unsigned int textID;
	unsigned int descriptionID;
	Color color;
	ItemType type;
	ItemCategory category;
	ItemFlag flag;
	unsigned long price;
	int damage;
	int damageDelta;
	int attackRate;
	unsigned int delay;
	int defense;
	int defenseRate;
	unsigned int requiredLevel;
	unsigned int requiredAbilities[Ability::count];
	std::vector<ItemEffect> effects;

public:
	Item(const std::string &filename, bool fullPath = false);
	unsigned int getTextID() const;
	unsigned int getDescriptionID() const;
	Color getColor() const;
	ItemType getType() const;
	ItemCategory getCategory() const;
	unsigned long getPrice() const;
	int getDamage() const;
	int getDamageDelta() const;
	int getAttackRate() const;
	int getDelay() const;
	int getDefense() const;
	int getDefenseRate() const;
	int getRequiredLevel() const;
	int getRequiredAbility(const Ability ability) const;
	bool saveToFile(const std::string& filename, bool fullPath = false);
};

struct Inventory {
private:
	std::vector<Item> backpack;
	std::map<Stack, Item> stack;

public:
	bool addItem(const std::string& filename);
	bool addItem(Item item);
	unsigned int getBackpackSize();
	Item* getBackpackItem(unsigned int name);
	bool isEmpty() const;
	bool isFull() const;
};

static const String typeTextIDs[(size_t)(ItemType::count)] = {
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
	String::Quick,
};

static const String categoryTextIDs[(size_t)(ItemCategory::count)] = {
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
	String::Spell,
};

#endif // ITEM_HPP
