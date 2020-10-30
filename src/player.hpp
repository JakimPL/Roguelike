#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "structures.hpp"

class Game;

class Player
{
private:
	std::string name;
	Position position;

public:
	Area* currentArea;
	Character character;
	Creature creature;

public:
	Player(std::string initialName);

	std::string getName();
	Position getPosition();
	void move(Direction direction);
};

#endif // PLAYER_HPP
