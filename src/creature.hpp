#ifndef CREATURE_HPP
#define CREATURE_HPP

#include "item.hpp"

enum Ability : size_t {
	strength,
	dexterity,
	constitution,
	intelligence,
	wisdom,
	count
};

enum class Race {
	undefined,
	human,
	elf,
	gnome,
	dwarf,
	orc,
	animal,
	monster,
	magic_being,
	demigod,
	god,
	count
};

enum class State {
	dead,
	sleeping,
	mute,
	frozen,
	panic,
	summoned,
	undead,
	count
};

enum class Gender {
	unknown,
	male,
	female,
	nonbinary,
	count
};

enum class CreatureCharacter {
	unknown,
	good,
	neutral,
	evil,
	count
};

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
	unsigned int abilities[Ability::count];
	unsigned int resistance[(unsigned int)(Elementals::count)];
	std::vector<CreatureEffect> effects;

public:
	Inventory inventory;

public:
	Creature();
	Creature(const std::string& filename);
	bool saveToFile(const std::string& filename);

	unsigned int getTextID() const;
	Color getColor() const;
	char getLetter() const;
	int getLevel() const;
	int getRemainingXP() const;
	int getCurrentXP() const;
	int getCurrentHP() const;
	int getCurrentMP() const;
	int getMaxHP() const;
	int getMaxMP() const;
	int getAbilityValue(const Ability ability) const;
	unsigned int getWeaponTextID();
};

#endif // CREATURE_HPP
