#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "structures.hpp"

class Player
{
private:
	Position position;

public:
	Character character;
	Creature creature;
	Player();

	Position getPosition();
};

#endif // PLAYER_HPP
