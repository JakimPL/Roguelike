#include "creature.hpp"
#include "color.hpp"
#include "log.hpp"
#include "text.hpp"

Creature::Creature(const std::string&)
{

}

Creature::Creature()
{
	nameID = 0;
	color = {255, 255, 0};
	letter = 'X';
	race = Race::human;
	gender = Gender::male;
	state = 0;
	level = 1;
	xpCurrent = 0;
	xpNextLevel = 100;
	xpValue = 0;
	gold = 50;
	hpCurrent = 30;
	hpBase = 28;
	hpRegenerationBase = 1;
	mpCurrent = 24;
	mpBase = 22;
	mpRegenerationBase = 0;
	damageBase = 1;
	attackRateBase = 24;
	defenseBase = 9;
	defenseRateBase = 0;
	abilityPoints = 10;
	abilitiesBase[strength] = 0;
	abilitiesBase[dexterity] = 0;
	abilitiesBase[constitution] = 0;
	abilitiesBase[intelligence] = 0;
	abilitiesBase[wisdom] = 0;
	effects = {};
	inventory.addItem("DAGGER");
	inventory.addItem("SHORTSWORD");
	inventory.addItem("LONGSWORD");
	inventory.addItem("BROADSWORD");
	inventory.addItem("LIGHTBLADE");
	updateStats();
}

void Creature::assignPoint(Ability ability)
{
	if (abilityPoints > 0) {
		abilitiesBase[ability]++;
		abilityPoints--;
		updateStats();
	}
}

bool Creature::equipItem(Item* item)
{
	bool success = false;
	if (item == nullptr) {
		_LogError("Trying to equip NULL item!");
		return false;
	}

	ItemType type = item->getType();
	Item* currentItem = inventory.getStackItem(type);

	inventory.dropItem(type);
	if (currentItem != item) {
		if (itemReqiuirementsSatisfied(item)) {
			inventory.equipItem(item);
			success = true;
		} else {
			if (currentItem != nullptr) {
				inventory.equipItem(currentItem);
			}
		}
	}

	updateStats();
	return success;
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

void Creature::updateStats()
{
	for (size_t abilityIndex = 0; abilityIndex < Ability::count; ++abilityIndex) {
		Ability ability = Ability(abilityIndex);
		abilities[ability] = abilitiesBase[ability];
	}

	hpMax = hpBase + abilities[Ability::constitution] + level * 2;
	mpMax = mpBase + abilities[Ability::intelligence] / 2 + level * 2;
	hpRegeneration = hpRegenerationBase + level / 3;
	mpRegeneration = mpRegenerationBase + level / 4;
	defense = defenseBase + abilities[Ability::constitution] / 5 + 1;
	defenseRate = defenseRateBase + abilities[Ability::dexterity] / 5 + 1;
	damageMin = damageBase + abilities[Ability::strength] / 5;
	damageMax = damageMin + abilities[Ability::strength] / 25 + 1;
	attackRate = attackRateBase + abilities[Ability::dexterity] / 4 + 1;

	for (size_t typeIndex = 0; typeIndex < size_t(ItemType::count); ++typeIndex) {
		ItemType type = ItemType(typeIndex);
		Item* item = inventory.getStackItem(type);
		if (item != nullptr and type != ItemType::quick and type != ItemType::quiver) {
			defense += item->getDefense();
			defenseRate += item->getDefenseRate();
			damageMin += item->getDamage();
			damageMax += item->getDamage() + item->getDamageDelta();
			attackRate += item->getAttackRate();
		}
	}
}

unsigned int Creature::getNameID() const
{
	return nameID;
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
	return xpNextLevel - xpCurrent;
}

int Creature::getXPCurrent() const
{
	return xpCurrent;
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
	Item* weaponItem = inventory.getStackItem(ItemType::weapon);
	if (weaponItem != nullptr) {
		return weaponItem->getNameID();
	}

	return (unsigned int)(String::Item::Fist);
}

bool Creature::saveToFile(const std::string&)
{
	return false;
}
