#include "creature.hpp"
#include "color.hpp"
#include "functions.hpp"
#include "log.hpp"
#include "message.hpp"
#include "text.hpp"

#include <math.h>

Creature::Creature(const std::string& filename, bool fullPath)
{
	loadFromFile(filename, fullPath);
}

Creature::Creature()
{
	nameID = 0;
	color = {255, 255, 0};
	letter = 'X';
	race = Race::human;
	gender = Gender::male;
	alignment = CreatureAlignment::neutral;
	state = State::none;
	level = 1;
	xpCurrent = 0;
	xpNextLevel = 100;
	xpValue = 0;
	gold = 5000;
	hpCurrent = 30;
	hpBase = 28;
	hpRegenerationBase = 1;
	mpCurrent = 24;
	mpBase = 22;
	mpRegenerationBase = 0;
	damageMinBase = 1;
	damageMaxBase = 1;
	attackRateBase = 19;
	defenseBase = 9;
	defenseRateBase = 0;
	abilityPoints = 10;
	abilitiesBase[strength] = 0;
	abilitiesBase[dexterity] = 0;
	abilitiesBase[constitution] = 0;
	abilitiesBase[intelligence] = 0;
	abilitiesBase[wisdom] = 0;
	effects = {};
	updateStats();
}

bool Creature::loadFromFile(const std::string& filename, bool fullPath)
{
	bool success;
	std::string path = fullPath ? filename : Functions::getPath(filename, Filetype::CRE);
	_LogInfo("Opening " << path << " creature file");
	std::ifstream resource(path, std::ios::in | std::ios::binary);
	if (resource.good()) {
		success = load(resource);
	} else {
		_LogError("Failed to open " << filename << " creature file!");
		success = false;
	}

	resource.close();
	return success;
}

bool Creature::saveToFile(const std::string& filename, bool fullPath)
{
	std::string path = fullPath ? filename : Functions::getPath(filename, Filetype::CRE);
	std::ofstream resource(path);
	if (resource.good()) {
		save(resource);

		_LogInfo("Saved " << path << " creature file succesfully");
		resource.close();
		return true;
	} else {
		_LogError("Failed to save " << path << " creature file!");
		return false;
	}
}

bool Creature::load(std::ifstream& resource)
{
	char resourceHeader[SIZE_HEADER + 1];
	Functions::read(resource, resourceHeader, SIZE_HEADER);
	if (Functions::compareHeaders(headerCRE, resourceHeader)) {
		resource.read(reinterpret_cast<char*>(&nameID), SIZE_INT);
		resource.read(reinterpret_cast<char*>(&letter), SIZE_CHAR);
		resource.read(reinterpret_cast<char*>(&color), SIZE_COLOR);
		resource.read(reinterpret_cast<char*>(&race), SIZE_CHAR);
		resource.read(reinterpret_cast<char*>(&gender), SIZE_CHAR);
		resource.read(reinterpret_cast<char*>(&alignment), SIZE_CHAR);
		resource.read(reinterpret_cast<char*>(&state), SIZE_CHAR);
		resource.read(reinterpret_cast<char*>(&level), SIZE_INT);
		resource.read(reinterpret_cast<char*>(&xpCurrent), SIZE_LONG);
		resource.read(reinterpret_cast<char*>(&xpNextLevel), SIZE_LONG);
		resource.read(reinterpret_cast<char*>(&xpValue), SIZE_LONG);
		resource.read(reinterpret_cast<char*>(&gold), SIZE_LONG);
		resource.read(reinterpret_cast<char*>(&hpCurrent), SIZE_INT);
		resource.read(reinterpret_cast<char*>(&hpBase), SIZE_INT);
		resource.read(reinterpret_cast<char*>(&hpRegenerationBase), SIZE_INT);
		resource.read(reinterpret_cast<char*>(&mpCurrent), SIZE_INT);
		resource.read(reinterpret_cast<char*>(&mpBase), SIZE_INT);
		resource.read(reinterpret_cast<char*>(&mpRegenerationBase), SIZE_INT);
		resource.read(reinterpret_cast<char*>(&damageMinBase), SIZE_INT);
		resource.read(reinterpret_cast<char*>(&damageMaxBase), SIZE_INT);
		resource.read(reinterpret_cast<char*>(&attackRateBase), SIZE_INT);
		resource.read(reinterpret_cast<char*>(&defenseBase), SIZE_INT);
		resource.read(reinterpret_cast<char*>(&defenseRateBase), SIZE_INT);
		resource.read(reinterpret_cast<char*>(&abilityPoints), SIZE_INT);
		for (size_t ability = 0; ability < Ability::count; ++ability) {
			resource.read(reinterpret_cast<char*>(&abilitiesBase[ability]), SIZE_INT);
		}

		// resistances and inventory to be implemented
		updateStats();
		_LogInfo("Creature file opened successfully.");
		return true;
	} else {
		_LogError("Invalid creature file!");
		return false;
	}
}

void Creature::save(std::ofstream& resource)
{
	resource.write(headerCRE, SIZE_HEADER);
	resource.write(reinterpret_cast<char*>(&nameID), SIZE_INT);
	resource.write(reinterpret_cast<char*>(&letter), SIZE_CHAR);
	resource.write(reinterpret_cast<char*>(&color), SIZE_COLOR);
	resource.write(reinterpret_cast<char*>(&race), SIZE_CHAR);
	resource.write(reinterpret_cast<char*>(&gender), SIZE_CHAR);
	resource.write(reinterpret_cast<char*>(&alignment), SIZE_CHAR);
	resource.write(reinterpret_cast<char*>(&state), SIZE_CHAR);
	resource.write(reinterpret_cast<char*>(&level), SIZE_INT);
	resource.write(reinterpret_cast<char*>(&xpCurrent), SIZE_LONG);
	resource.write(reinterpret_cast<char*>(&xpNextLevel), SIZE_LONG);
	resource.write(reinterpret_cast<char*>(&xpValue), SIZE_LONG);
	resource.write(reinterpret_cast<char*>(&gold), SIZE_LONG);
	resource.write(reinterpret_cast<char*>(&hpCurrent), SIZE_INT);
	resource.write(reinterpret_cast<char*>(&hpBase), SIZE_INT);
	resource.write(reinterpret_cast<char*>(&hpRegenerationBase), SIZE_INT);
	resource.write(reinterpret_cast<char*>(&mpCurrent), SIZE_INT);
	resource.write(reinterpret_cast<char*>(&mpBase), SIZE_INT);
	resource.write(reinterpret_cast<char*>(&mpRegenerationBase), SIZE_INT);
	resource.write(reinterpret_cast<char*>(&damageMinBase), SIZE_INT);
	resource.write(reinterpret_cast<char*>(&damageMaxBase), SIZE_INT);
	resource.write(reinterpret_cast<char*>(&attackRateBase), SIZE_INT);
	resource.write(reinterpret_cast<char*>(&defenseBase), SIZE_INT);
	resource.write(reinterpret_cast<char*>(&defenseRateBase), SIZE_INT);
	resource.write(reinterpret_cast<char*>(&abilityPoints), SIZE_INT);
	for (size_t ability = 0; ability < Ability::count; ++ability) {
		resource.write(reinterpret_cast<char*>(&abilitiesBase[ability]), SIZE_INT);
	}

	// resistances and inventory to be implemented
}

void Creature::assignPoint(Ability ability)
{
	if (abilityPoints > 0) {
		abilitiesBase[ability]++;
		abilityPoints--;
		updateStats();
	}
}

bool Creature::equipItem(int index)
{
	bool success = false;

	Item* item = inventory.getBackpackItem(index);
	ItemType type = item->getType();
	int currentItemIndex = inventory.getStackItemIndex(type);

	inventory.dropItem(type);
	if (index != currentItemIndex) {
		if (itemReqiuirementsSatisfied(item)) {
			inventory.equipItem(index);
			success = true;
		} else {
			if (currentItemIndex != -1) {
				inventory.equipItem(currentItemIndex);
			}
		}
	} else {
		success = true;
	}

	updateStats();
	return success;
}

bool Creature::itemReqiuirementsSatisfied(Item* item)
{
	if (item == nullptr) {
		return true;
	}

	if (item->getRequiredLevel() > level) {
		return false;
	}

	for (size_t abilityIndex = 0; abilityIndex < Ability::count; ++abilityIndex) {
		Ability ability = Ability(abilityIndex);
		if (item->getRequiredAbility(ability) > getAbilityValue(ability)) {
			return false;
		}
	}

	return true;
}

bool Creature::isItemEquipped(int index)
{
	Item* item = inventory.getBackpackItem(index);
	return inventory.getStackItemIndex(item->getType()) == index;
}

void Creature::updateStats()
{
	for (size_t abilityIndex = 0; abilityIndex < Ability::count; ++abilityIndex) {
		Ability ability = Ability(abilityIndex);
		abilities[ability] = abilitiesBase[ability];
	}

	hpMax = hpBase + abilities[Ability::constitution] + level * 2;
	mpMax = mpBase + abilities[Ability::intelligence] / 2 + level * 2;
	hpRegeneration = hpRegenerationBase + abilities[Ability::constitution] / 5 + level / 3;
	mpRegeneration = mpRegenerationBase + abilities[Ability::intelligence] / 5 + level / 4;
	defense = defenseBase + abilities[Ability::constitution] / 5 + 1;
	defenseRate = defenseRateBase + abilities[Ability::dexterity] / 5 + 1;
	damageMin = damageMinBase + abilities[Ability::strength] / 5;
	damageMax = damageMaxBase + abilities[Ability::strength] / 25 + 1;
	attackRate = attackRateBase + abilities[Ability::dexterity] / 4 + 1;

	for (size_t typeIndex = 0; typeIndex < size_t(ItemType::count); ++typeIndex) {
		ItemType type = ItemType(typeIndex);
		int index = inventory.getStackItemIndex(type);
		Item* item = inventory.getBackpackItem(index);
		if (item != nullptr and type != ItemType::quick and type != ItemType::quiver) {
			defense += item->getDefense();
			defenseRate += item->getDefenseRate();
			damageMin += item->getDamage();
			damageMax += item->getDamage() + item->getDamageDelta();
			attackRate += item->getAttackRate();
		}
	}
}

Color Creature::getColor() const
{
	return color;
}

Race Creature::getRace() const
{
	return race;
}

Gender Creature::getGender() const
{
	return gender;
}

CreatureAlignment Creature::getAlignment() const
{
	return alignment;
}

State Creature::getState() const
{
	return state;
}

unsigned int Creature::getNameID() const
{
	return nameID;
}

char Creature::getLetter() const
{
	return letter;
}

int Creature::getBaseHP() const
{
	return hpBase;
}

int Creature::getBaseMP() const
{
	return mpBase;
}

int Creature::getBaseHPRegeneration() const
{
	return hpRegenerationBase;
}

int Creature::getBaseMPRegeneration() const
{
	return mpRegenerationBase;
}

int Creature::getBaseDefense() const
{
	return defenseBase;
}

int Creature::getBaseDefenseRate() const
{
	return defenseRateBase;
}

int Creature::getBaseDamageMin() const
{
	return damageMinBase;
}

int Creature::getBaseDamageMax() const
{
	return damageMaxBase;
}

int Creature::getBaseAbilityValue(const Ability ability) const
{
	return abilitiesBase[ability];
}

bool Creature::isNextLevel() const
{
	return getXPRemaining() <= 0;
}

int Creature::getLevel() const
{
	return level;
}

int Creature::getXPRemaining() const
{
	return xpNextLevel - xpCurrent;
}

int Creature::getXPCurrent() const
{
	return xpCurrent;
}

int Creature::getXPValue() const
{
	return xpValue;
}

int Creature::getHPCurrent() const
{
	return hpCurrent;
}

int Creature::getHPMax() const
{
	return hpMax;
}

int Creature::getHPRegeneration() const
{
	return hpRegeneration;
}

int Creature::getMPCurrent() const
{
	return mpCurrent;
}

int Creature::getMPMax() const
{
	return mpMax;
}

int Creature::getMPRegeneration() const
{
	return mpRegeneration;
}

int Creature::getDefense() const
{
	return defense;
}

int Creature::getDefenseRate() const
{
	return defenseRate;
}

int Creature::getDamageMin() const
{
	return damageMin;
}

int Creature::getDamageMax() const
{
	return damageMax;
}

int Creature::getAttackRate() const
{
	return attackRate;
}

int Creature::getGold() const
{
	return gold;
}

int Creature::getAbilityPoints() const
{
	return abilityPoints;
}

int Creature::getAbilityValue(const Ability ability) const
{
	return abilities[ability];
}

unsigned int Creature::getWeaponNameID()
{
	int index = inventory.getStackItemIndex(ItemType::weapon);
	if (index != -1) {
		Item* weaponItem = inventory.getBackpackItem(index);
		return weaponItem->getNameID();
	}

	return s_Fist % TEXT_CATEGORY_SIZE;
}

void Creature::setNameID(unsigned int value)
{
	nameID = value;
}

void Creature::setColor(Color value)
{
	color = value;
}

void Creature::setRace(Race value)
{
	race = value;
}

void Creature::setGender(Gender value)
{
	gender = value;
}

void Creature::setAlignment(CreatureAlignment value)
{
	alignment = value;
}

void Creature::setState(State value)
{
	state = value;
}

void Creature::setLetter(char value)
{
	letter = value;
}

void Creature::setLevel(unsigned int value)
{
	level = value;
}

void Creature::setXPCurrent(int value)
{
	xpCurrent = value;
}

void Creature::setXPNextLevel(int value)
{
	xpNextLevel = value;
}

void Creature::setXPValue(unsigned int value)
{
	xpValue = value;
}

void Creature::setHP(int value)
{
	hpCurrent = value;
}

void Creature::setMP(int value)
{
	mpCurrent = value;
}

void Creature::setBaseHP(int value)
{
	hpBase = value;
}

void Creature::setBaseMP(int value)
{
	mpBase = value;
}

void Creature::setBaseHPRegeneration(int value)
{
	hpRegenerationBase = value;
}

void Creature::setBaseMPRegeneration(int value)
{
	mpRegenerationBase = value;
}

void Creature::setBaseDefense(int value)
{
	defenseBase = value;
}

void Creature::setBaseDefenseRate(int value)
{
	defenseRateBase = value;
}

void Creature::setBaseDamageMin(int value)
{
	damageMinBase = value;
}

void Creature::setBaseDamageMax(int value)
{
	damageMaxBase = value;
}

void Creature::setBaseAttackRate(int value)
{
	attackRateBase = value;
}

void Creature::setBaseAbilityValue(const Ability ability, int value)
{
	abilitiesBase[ability] = value;
}

void Creature::addGold(int amount)
{
	gold += amount;
}

void Creature::addXP(int amount)
{
	xpCurrent += amount;
	if (isNextLevel()) {
		nextLevel();
	}
}

void Creature::nextLevel()
{
	double k = 1.2f;
	xpNextLevel = floor(xpNextLevel * (k + (k - 1) / (pow(k, level) - 1)));
	abilityPoints += 5;
	level++;

	updateStats();
}

void Creature::takeGold(int amount)
{
	gold -= amount;
}

void Creature::takeHP(int amount)
{
	hpCurrent -= amount;
}
