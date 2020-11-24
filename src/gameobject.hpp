#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include "area.hpp"

class GameObject
{
protected:
	unsigned int nameID = 0;
	Position position = {0, 0, SOUTH};
	unsigned int delay = 0;
	bool movable = false;

	Color color;
	char letter;

public:
	Area* currentArea;

public:
	GameObject();

	Position getPosition() const;
	void setDirection(Direction direction);
	void move(Direction direction);
	void step();
};

#endif // GAMEOBJECT_HPP
