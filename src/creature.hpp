#ifndef CREATURE_HPP
#define CREATURE_HPP

#include "constants.hpp"
#include "item.hpp"
#include "structures.hpp"

struct Creature {
private:
	unsigned int textID;
	char letter;
	Color color;
	Race race;
	Gender gender;
	unsigned int state;
	unsigned int level;
	unsigned long xp;
	unsigned long xpNextLevel;
	unsigned long xpValue;
	unsigned long gold;
	unsigned int hp;
	unsigned int hpMax;
	unsigned int mp;
	unsigned int mpMax;
	unsigned int acBase;
	unsigned int accuracy;
	unsigned int strength;
	unsigned int dexterity;
	unsigned int constitution;
	unsigned int intelligence;
	unsigned int wisdom;
	unsigned int resistance[(unsigned int)(Elementals::count)];
	std::vector<CreatureEffect> effects;
	std::vector<Ability> abilities;

public:
	Inventory inventory;

public:
	Creature();
	Creature(const std::string& filename);
	bool saveToFile(const std::string& filename);

	unsigned int getTextID();
	Color getColor();
	char getLetter();
	int getLevel();
	int getRemainingXP();
	int getCurrentXP();
	int getCurrentHP();
	int getCurrentMP();
	int getMaxHP();
	int getMaxMP();
	unsigned int getWeaponTextID();
};

#endif // CREATURE_HPP
