#include "player.hpp"

Player::Player(std::string initialName)
{
	name = initialName;
	position.x = 0;
	position.y = 0;
	position.direction = SOUTH;
}

std::string Player::getName()
{
	return name;
}

Position Player::getPosition()
{
	return position;
}

void Player::move(Direction direction)
{
	position.direction = direction;
	if (!currentArea->getTile(position + 1).obstacle) {
		position.moveInDirection();
	}
}
