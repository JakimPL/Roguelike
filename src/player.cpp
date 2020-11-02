#include "player.hpp"

Player::Player(std::string initialName)
{
	name = initialName;
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
		if (delay == 0) {
			position.moveInDirection();
			delay = 10;
		}
	}
}

void Player::setDirection(Direction direction)
{
	position.direction = direction;
}

void Player::step()
{
	if (delay > 0) {
		delay--;
	}
}
