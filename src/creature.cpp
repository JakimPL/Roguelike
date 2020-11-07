#include "creature.hpp"
#include "color.hpp"
#include "log.hpp"
#include "text.hpp"

Creature::Creature(const std::string&)
{

}

Creature::Creature()
{
	textID = 0;
	color = {255, 255, 0};
	letter = 'X';
	race = Race::human;
	gender = Gender::male;
	state = 0;
	level = 1;
	xp = 0;
	xpNextLevel = 100;
	xpValue = 0;
	gold = 50;
	hp = 30;
	hpMax = 28;
	hpRegeneration = 1;
	mp = 24;
	mpMax = 22;
	mpRegeneration = 0;
	damage = 1;
	attackRate = 24;
	defense = 9;
	defenseRate = 0;
	abilityPoints = 10;
	abilities[strength] = 0;
	abilities[dexterity] = 0;
	abilities[constitution] = 0;
	abilities[intelligence] = 0;
	abilities[wisdom] = 0;
	effects = {};
	inventory.addItem("DAGGER");
	inventory.addItem("SHORTSWORD");
	inventory.addItem("LONGSWORD");
	inventory.addItem("BROADSWORD");
	inventory.addItem("LIGHTBLADE");
}

void Creature::assignPoint(Ability ability)
{
	if (abilityPoints > 0) {
		abilities[ability]++;
		abilityPoints--;
	}
}

bool Creature::equipItem(Item* item)
{
	if (item == nullptr) {
		_LogError("Trying to equip NULL item!");
		return false;
	}

	ItemType type = item->getType();
	Item* currentItem = inventory.getStackItem(type);

	if (currentItem == item) {
		inventory.dropItem(type);
		return false;
	}

	inventory.dropItem(type);
	if (itemReqiuirementsSatisfied(item)) {
		inventory.equipItem(item);
		return true;
	} else {
		if (currentItem != nullptr) {
			inventory.equipItem(currentItem);
		}

		return false;
	}
}

bool Creature::equipItem(unsigned int index)
{
	return equipItem(inventory.getBackpackItem(index));
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

bool Creature::isItemEquipped(Item *item)
{
	return inventory.getStackItem(item->getType()) == item;
}

unsigned int Creature::getTextID() const
{
	return textID;
}

char Creature::getLetter() const
{
	return letter;
}

Color Creature::getColor() const
{
	return color;
}

int Creature::getLevel() const
{
	return level;
}

int Creature::getXPRemaining() const
{
	return xpNextLevel - xp;
}

int Creature::getXPCurrent() const
{
	return xp;
}

int Creature::getHPCurrent() const
{
	return hp;
}

int Creature::getHPMax() const
{
	return hpMax + abilities[Ability::constitution] + level * 2;
}

int Creature::getHPRegeneration() const
{
	return hpRegeneration;
}

int Creature::getMPCurrent() const
{
	return mp;
}

int Creature::getMPMax() const
{
	return mpMax + abilities[Ability::intelligence] / 2 + level * 2;
}

int Creature::getMPRegeneration() const
{
	return mpRegeneration;
}

int Creature::getDefense() const
{
	return defense + abilities[Ability::constitution] / 5 + 1;
}

int Creature::getDefenseRate() const
{
	return defenseRate + abilities[Ability::dexterity] / 5 + 1;
}

int Creature::getDamageMin() const
{
	return damage;
}

int Creature::getDamageMax() const
{
	return damage;
}

int Creature::getAttackRate() const
{
	return attackRate + abilities[Ability::dexterity] / 4 + 1;
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

unsigned int Creature::getWeaponTextID()
{
	return (unsigned int)(String::Item::Fist);
}

bool Creature::saveToFile(const std::string&)
{
	return false;
}
