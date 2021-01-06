#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include "src/color.hpp"
#include "src/structures.hpp"
#include "src/text.hpp"

enum class ObjectType : unsigned char {
	None,
	Player,
	Item,
	NPC,
	Door,
	Sign
};

class GameObject;

struct GameObjects : public std::vector<GameObject*> {
	void deleteObject(GameObject* object);
	void deleteObjects();
};

class GameObject
{
protected:
	std::vector<GameObject*>& objects;
	unsigned int nameID = 0;
	unsigned int delay = 0;

	bool movable = false;
	bool solid = false;
	bool transparent = false;

	Position position = {0, 0, SOUTH};
	Color color;
	char letter;

public:
	ObjectType type = ObjectType::None;

public:
	GameObject(GameObjects& gameObjects);
	virtual ~GameObject();

	bool exists(GameObject* object);
	void remove(GameObject* object);

	unsigned int getNameID() const;
	Color getColor() const;
	char getLetter() const;
	Position getPosition() const;
	bool isSolid() const;
	bool isTransparent() const;

	void setNameID(unsigned int index);
	void setColor(Color newColor);
	void setLetter(char newLetter);
	void setDirection(Direction direction);

	void move(Direction direction);
	void move();
	void step();

	virtual TextPair getText();
	virtual void load(std::ifstream& resource);
	virtual void save(std::ofstream& resource);
};

#endif // GAMEOBJECT_HPP
