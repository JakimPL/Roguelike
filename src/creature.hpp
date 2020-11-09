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
	construct,
	demigod,
	god,
	count
};

enum class Gender {
	unknown,
	male,
	female,
	nonbinary,
	count
};

enum class State {
	none,
	dead,
	unconscious,
	paralyzed,
	sleeping,
	mute,
	frozen,
	panic,
	summoned,
	undead,
	count
};

enum class CreatureAlignment {
	unknown,
	good,
	neutral,
	evil,
	count
};

struct Creature {
private:
	unsigned int nameID;
	char letter;
	Color color;
	Race race;
	Gender gender;
	CreatureAlignment alignment;
	State state;
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
	int damageMinBase;
	int damageMaxBase;
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
	Creature(const std::string& filename, bool fullPath = false);
	bool saveToFile(const std::string& filename, bool fullPath = false);

	void assignPoint(Ability ability);
	bool equipItem(unsigned int index);
	bool equipItem(Item* item);
	bool itemReqiuirementsSatisfied(Item* item);
	bool isItemEquipped(Item* item);
	void updateStats();

	unsigned int getNameID() const;
	Color getColor() const;
	Race getRace() const;
	Gender getGender() const;
	CreatureAlignment getAlignment() const;
	State getState() const;
	char getLetter() const;

	int getBaseHP() const;
	int getBaseMP() const;
	int getBaseHPRegeneration() const;
	int getBaseMPRegeneration() const;
	int getBaseDefense() const;
	int getBaseDefenseRate() const;
	int getBaseDamageMin() const;
	int getBaseDamageMax() const;
	int getBaseAttackRate() const;
	int getBaseAbilityValue(const Ability ability) const;

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
	unsigned int getWeaponNameID();

	void setNameID(unsigned int value);
	void setColor(Color value);
	void setRace(Race value);
	void setGender(Gender value);
	void setAlignment(CreatureAlignment value);
	void setState(State value);
	void setLetter(char value);
	void setLevel(unsigned int value);
	void setXPCurrent(int value);
	void setXPNextLevel(int value);
	void setHP(int value);
	void setMP(int value);
	void setBaseHP(int value);
	void setBaseMP(int value);
	void setBaseHPRegeneration(int value);
	void setBaseMPRegeneration(int value);
	void setBaseDefense(int value);
	void setBaseDefenseRate(int value);
	void setBaseDamageMin(int value);
	void setBaseDamageMax(int value);
	void setBaseAttackRate(int value);
	void setBaseAbilityValue(const Ability ability, int value);
};

#endif // CREATURE_HPP
