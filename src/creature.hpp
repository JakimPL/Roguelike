#ifndef CREATURE_HPP
#define CREATURE_HPP

#include "item.hpp"

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
	int level;
	long xpCurrent;
	long xpNextLevel;
	long xpValue;
	long gold;
	int hpCurrent;
	int hpMax;
	int hpRegeneration;
	int hpBase;
	int hpRegenerationBase;
	int mpCurrent;
	int mpMax;
	int mpRegeneration;
	int mpBase;
	int mpRegenerationBase;
	int damageMin;
	int damageMax;
	int damageBase;
	int attackRate;
	int attackRateBase;
	int defense;
	int defenseBase;
	int defenseRate;
	int defenseRateBase;
	int abilityPoints;
	int abilities[Ability::count];
	int abilitiesBase[Ability::count];
	int resistance[(unsigned int)(Elementals::count)];
	std::vector<CreatureEffect> effects;
public:
	Inventory inventory;

public:
	Creature();
	Creature(const std::string& filename);
	bool saveToFile(const std::string& filename);

	void assignPoint(Ability ability);
	bool equipItem(unsigned int index);
	bool equipItem(Item* item);
	bool itemReqiuirementsSatisfied(Item* item);
	bool isItemEquipped(Item* item);
	void updateStats();

	unsigned int getTextID() const;
	Color getColor() const;
	char getLetter() const;
	int getLevel() const;
	int getXPRemaining() const;
	int getXPCurrent() const;
	int getHPCurrent() const;
	int getHPMax() const;
	int getHPRegeneration() const;
	int getMPCurrent() const;
	int getMPMax() const;
	int getMPRegeneration() const;
	int getDefense() const;
	int getDefenseRate() const;
	int getDamageMin() const;
	int getDamageMax() const;
	int getAttackRate() const;
	int getGold() const;
	int getAbilityPoints() const;
	int getAbilityValue(const Ability ability) const;
	unsigned int getWeaponTextID();
};

#endif // CREATURE_HPP
