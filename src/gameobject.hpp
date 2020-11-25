#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include "area.hpp"

class GameObject
{
protected:
	std::vector<GameObject*>& objects;
	unsigned int nameID = 0;
	unsigned int delay = 0;

	bool movable = false;
	bool solid = false;

	Position position = {0, 0, SOUTH};
	Color color;
	char letter;

public:
	Area* currentArea;

public:
	GameObject(std::vector<GameObject*>& gameObjects);

	unsigned int getNameID() const;
	Color getColor() const;
	char getLetter() const;
	Position getPosition() const;
	bool isPositionFree(int x, int y) const;
	bool isPositionFree(Position position) const;
	std::vector<GameObject*> isPositionTaken(int x, int y) const;
	std::vector<GameObject*> isPositionTaken(Position position) const;

	void setDirection(Direction direction);
	void move(Direction direction);
	void step();
};

typedef std::vector<GameObject*> GameObjects;

#endif // GAMEOBJECT_HPP
