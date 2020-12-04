#ifndef ITEMOBJECT_HPP
#define ITEMOBJECT_HPP

#include "gameobject.hpp"
#include "src/item.hpp"

class ItemObject : public GameObject
{
public:
	std::string resourceName;
	Item item;

public:
	ItemObject(GameObjects& gameObjects, std::string resourceName, Position initialPosition);
	ItemObject(GameObjects& gameObjects, std::ifstream& resource);

	TextPair getText() override;
	void load(std::ifstream& resource) override;
	void save(std::ofstream& resource) override;
};

#endif // ITEMOBJECT_HPP
