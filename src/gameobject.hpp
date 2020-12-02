#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include "area.hpp"
#include "text.hpp"

enum class ObjectType {
	None,
	Player,
	Item,
	NPC,
	Door
};

class GameObject;

struct GameObjects : public std::vector<GameObject*> {
	void deleteObject(GameObject* object);
};

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
	ObjectType type = ObjectType::None;
	Area* currentArea;

public:
	GameObject(GameObjects& gameObjects);
	virtual ~GameObject();

	unsigned int getNameID() const;
	Color getColor() const;
	char getLetter() const;
	Position getPosition() const;
	bool isPositionFree(int x, int y) const;
	bool isPositionFree(Position position) const;
	GameObjects isPositionTaken(int x, int y) const;
	GameObjects isPositionTaken(Position position) const;

	void setDirection(Direction direction);
	void move(Direction direction);
	void step();

	virtual TextPair getText();
	virtual void load(std::ifstream& resource);
	virtual void save(std::ofstream& resource);
};

#endif // GAMEOBJECT_HPP
