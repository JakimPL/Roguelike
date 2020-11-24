#include "player.hpp"

Player::Player(Creature initialCreature, std::string initialName) : creature(initialCreature)
{
	name = initialName;
	movable = true;

	color = creature.getColor();
	letter = creature.getLetter();
}

Player::Player(std::string initialName) : creature()
{
	name = initialName;
	movable = true;

	color = creature.getColor();
	letter = creature.getLetter();
}

std::string Player::getName() const
{
	return name;
}
