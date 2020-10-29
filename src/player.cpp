#include "player.hpp"

Player::Player()
{
	position.x = 0;
	position.y = 0;
	position.direction = SOUTH;
}

Position Player::getPosition()
{
	return position;
}
