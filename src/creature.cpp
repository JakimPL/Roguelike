#include "creature.hpp"
#include "color.hpp"
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
	hpMax = 30;
	mp = 15;
	mpMax = 15;
	acBase = 10;
	accuracy = 25;
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

int Creature::getRemainingXP() const
{
	return xpNextLevel - xp;
}

int Creature::getCurrentXP() const
{
	return xp;
}

int Creature::getCurrentHP() const
{
	return hp;
}

int Creature::getCurrentMP() const
{
	return mp;
}

int Creature::getMaxHP() const
{
	return hpMax;
}

int Creature::getMaxMP() const
{
	return mpMax;
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
