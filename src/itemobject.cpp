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

ItemObject::ItemObject(GameObjects& gameObjects, std::ifstream& resource) : GameObject(gameObjects)
{
	type = ObjectType::Item;
	load(resource);
}

TextPair ItemObject::getText()
{
	return Text::makeTextPair(String::Item);
}

void ItemObject::load(std::ifstream& resource)
{
	item.load(resource);
}

void ItemObject::save(std::ofstream& resource)
{
	item.save(resource);
}
