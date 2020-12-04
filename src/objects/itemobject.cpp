#include "itemobject.hpp"
#include "src/functions.hpp"
#include "src/options.hpp"
#include "src/text.hpp"

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
	item = Item(resourceName);
}

TextPair ItemObject::getText()
{
	return Text::makeTextPair(String::Item);
}

void ItemObject::load(std::ifstream& resource)
{
	GameObject::load(resource);
	unsigned int size;
	resource.read(reinterpret_cast<char*>(&size), SIZE_INT);
	resourceName = Functions::readString(resource, size);
}

void ItemObject::save(std::ofstream& resource)
{
	GameObject::save(resource);
	unsigned int size = resourceName.size();
	resource.write(reinterpret_cast<char*>(&size), SIZE_INT);
	resource.write(resourceName.c_str(), size);
}
