#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "structures.hpp"

class Player
{
private:
	std::string name;
	Position position;

public:
	Character character;
	Creature creature;
public:
	Player(std::string initialName);

	std::string getName();
	Position getPosition();
};

#endif // PLAYER_HPP
