#include "player.hpp"

Player::Player(GameObjects& gameObjects, Creature initialCreature, Position initialPosition, std::string initialName, Area* initialArea) : GameObject(gameObjects), creature(initialCreature)
{
	type = ObjectType::Player;
	name = initialName;
	movable = true;
	position = initialPosition;
	color = creature.getColor();
	letter = creature.getLetter();
	currentArea = initialArea;
}

std::string Player::getName() const
{
	return name;
}
