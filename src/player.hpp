#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "area.hpp"
#include "creature.hpp"

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

	std::string getName() const;
	Position getPosition() const;
	void setDirection(Direction direction);
	void move(Direction direction);
	void step();
};

#endif // PLAYER_HPP
