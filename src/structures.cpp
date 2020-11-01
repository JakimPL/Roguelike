#include "structures.hpp"

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

bool Character::saveToFile(const std::string&)
{
	return false;
}

bool Timer::frame()
{
	if (delta > 1000 / fps) {
		end = start;
		return true;
	}

	return false;
}

void Timer::update()
{
	start = SDL_GetTicks();
	delta = start - end;
}
