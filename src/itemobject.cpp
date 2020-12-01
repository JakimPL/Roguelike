#include "itemobject.hpp"
#include "options.hpp"
#include "text.hpp"

ItemObject::ItemObject(GameObjects& gameObjects, Item initialItem, Position initialPosition) : GameObject(gameObjects), item(initialItem)
{
	type = ObjectType::Item;
	solid = true;
	position = initialPosition;
	nameID = item.getNameID();
	color = item.getColor();
	letter = options.game.itemLetter;
}

TextPair ItemObject::getText()
{
	return Text::makeTextPair(String::Item);
}

bool ItemObject::load(std::ifstream& resource)
{
	return false;
}

bool ItemObject::save(std::ofstream& resource)
{
	return false;
}
