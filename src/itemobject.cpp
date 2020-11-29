#include "itemobject.hpp"
#include "text.hpp"

ItemObject::ItemObject(GameObjects& gameObjects, Item initialItem, Position initialPosition) : GameObject(gameObjects), item(initialItem)
{
	solid = true;
	position = initialPosition;
	nameID = item.getNameID();
	color = item.getColor();
	letter = ITEM_LETTER;
}