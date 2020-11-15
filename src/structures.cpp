#include "structures.hpp"
#include "functions.hpp"
#include "log.hpp"

void Position::moveInDirection(int length, Direction chosenDirection)
{
	int dir = (chosenDirection == COUNT ? Direction(direction) : chosenDirection);
	x += length * DIR_X(dir);
	y += length * DIR_Y(dir);
}

const Position Position::operator +(int length) const
{
	Position newPosition(*this);
	newPosition.moveInDirection(length);
	return newPosition;
}

const Position Position::operator -(int length) const
{
	return (*this) + (-length);
}

const Position Position::operator +(Position position) const
{
	Position newPosition(*this);
	newPosition.x += position.x;
	newPosition.y += position.y;
	return newPosition;
}

const Position Position::operator -(Position position) const
{
	Position newPosition(*this);
	newPosition.x -= position.x;
	newPosition.y -= position.y;
	return newPosition;
}

bool Character::saveToFile(const std::string&)
{
	return false;
}
