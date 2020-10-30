#ifndef ITEM_HPP
#define ITEM_HPP

#include "structures.hpp"

struct Item {
private:
	unsigned int textID;
	unsigned int descriptionID;
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
	bool saveToFile(const std::string& filename);
};

struct Inventory {
private:
	std::vector<Item> backpack;
	std::map<Stack, Item> stack;

public:
	bool addItem(const std::string& filename);
	bool addItem(Item item);
	bool isFull() const;
};

#endif // ITEM_HPP
