#include "player.hpp"

Player::Player(GameObjects& gameObjects, Creature initialCreature, std::string initialName) : GameObject(gameObjects), creature(initialCreature)
{
	name = initialName;
	movable = true;

	color = creature.getColor();
	letter = creature.getLetter();
}

Player::Player(GameObjects& gameObjects, std::string initialName) : GameObject(gameObjects), creature()
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
