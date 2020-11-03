#include "creature.hpp"

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
	strength = 0;
	dexterity = 0;
	constitution = 0;
	intelligence = 0;
	wisdom = 0;
	effects = {};
	abilities = {};
	inventory.addItem("DAGGER");
	inventory.addItem("DAGGER");
}

unsigned int Creature::getTextID()
{
	return textID;
}

char Creature::getLetter()
{
	return letter;
}

Color Creature::getColor()
{
	return color;
}

int Creature::getLevel()
{
	return level;
}

int Creature::getRemainingXP()
{
	return xpNextLevel - xp;
}

int Creature::getCurrentXP()
{
	return xp;
}

int Creature::getCurrentHP()
{
	return hp;
}

int Creature::getCurrentMP()
{
	return mp;
}

int Creature::getMaxHP()
{
	return hpMax;
}

int Creature::getMaxMP()
{
	return mpMax;
}

unsigned int Creature::getWeaponTextID()
{
	return (unsigned int)(String::Fist);
}

bool Creature::saveToFile(const std::string&)
{
	return false;
}
