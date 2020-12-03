#include "itemobject.hpp"
#include "options.hpp"
#include "text.hpp"

ItemObject::ItemObject(GameObjects& gameObjects, std::string initialResourceName, Position initialPosition) : GameObject(gameObjects), resourceName(initialResourceName), item(initialResourceName)
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
	unsigned int size;
	resource.read(reinterpret_cast<char*>(&size), SIZE_INT);
	resource.read(reinterpret_cast<char*>(&resourceName), size);
}

void ItemObject::save(std::ofstream& resource)
{
	unsigned int size = resourceName.size();
	resource.write(reinterpret_cast<char*>(&size), SIZE_INT);
	resource.write(reinterpret_cast<char*>(&resourceName), size);
}
