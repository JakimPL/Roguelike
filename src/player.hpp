#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "area.hpp"
#include "structures.hpp"
#include "creature.hpp"

class Game;

class Player
{
private:
	std::string name;
	Position position = {0, 0, SOUTH};
	unsigned int delay = 0;

public:
	Area* currentArea;
	Character character;
	Creature creature;

public:
	Player(std::string initialName);

	std::string getName();
	Position getPosition();
	void setDirection(Direction direction);
	void move(Direction direction);
	void step();
};

#endif // PLAYER_HPP
