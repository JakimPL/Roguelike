#ifndef ITEM_HPP
#define ITEM_HPP

#include "structures.hpp"

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

struct ItemEffect {
	unsigned int textID;
};

struct Item {
private:
	unsigned int textID;
	unsigned int descriptionID;
	Color color;
	ItemType type;
	ItemCategory category;
	ItemFlag flag;
	unsigned long price;
	int attack;
	int attackRate;
	int damage;
	int damageDelta;
	unsigned int delay;
	int defense;
	int defenseRate;
	unsigned int requiredLevel;
	unsigned int requiredStrength;
	unsigned int requiredWisdom;
	unsigned int requiredDexterity;
	unsigned int requiredIntelligence;
	std::vector<ItemEffect> effects;

public:
	Item(const std::string &filename);
	unsigned int getTextID();
	Color getColor();
	bool saveToFile(const std::string& filename);
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
	bool isFull() const;
};

#endif // ITEM_HPP
