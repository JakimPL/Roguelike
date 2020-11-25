#ifndef ITEMOBJECT_HPP
#define ITEMOBJECT_HPP

#include "gameobject.hpp"
#include "item.hpp"

class ItemObject : public GameObject
{
public:
	Item item;

public:
	ItemObject(GameObjects& gameObjects, Item initialItem, Position initialPosition);
};

#endif // ITEMOBJECT_HPP
