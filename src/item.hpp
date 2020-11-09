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
	unidentified,
	count
};

struct ItemEffect {
	unsigned int nameID;
};

struct Item {
private:
	unsigned int nameID;
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
	Item();
	Item(const std::string& filename, bool fullPath = false);
	unsigned int getNameID() const;
	unsigned int getDescriptionID() const;
	Color getColor() const;
	ItemType getType() const;
	ItemCategory getCategory() const;
	ItemFlag getFlag() const;
	unsigned long getPrice() const;
	int getDamage() const;
	int getDamageDelta() const;
	int getAttackRate() const;
	int getDelay() const;
	int getDefense() const;
	int getDefenseRate() const;
	int getRequiredLevel() const;
	int getRequiredAbility(const Ability ability) const;

	void setNameID(unsigned int value);
	void setDescriptionID(unsigned int value);
	void setColor(Color value);
	void setType(ItemType value);
	void setCategory(ItemCategory value);
	void setFlag(ItemFlag value);
	void setPrice(unsigned long value);
	void setDamage(int value);
	void setDamageDelta(int value);
	void setAttackRate(int value);
	void setDelay(unsigned int value);
	void setDefense(int value);
	void setDefenseRate(int value);
	void setRequiredLevel(unsigned int value);
	void setRequiredAbility(const Ability ability, unsigned int value);

	bool saveToFile(const std::string& filename, bool fullPath = false);
};

struct Inventory {
private:
	std::vector<Item> backpack;
	std::map<ItemType, Item*> stack;

public:
	bool addItem(const std::string& filename);
	bool addItem(Item item);
	void dropItem(ItemType type);
	void dropItem(Item* item);
	void equipItem(Item* item);

	unsigned int getBackpackSize();
	Item* getBackpackItem(unsigned int index);
	Item* getStackItem(ItemType type);

	bool isEmpty() const;
	bool isFull() const;
};

#endif // ITEM_HPP
