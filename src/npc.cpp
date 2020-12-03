#include "npc.hpp"

NPC::NPC(GameObjects& gameObjects, std::string initialResourceName, Position initialPosition) : GameObject(gameObjects), resourceName(initialResourceName), creature(resourceName)
{
	type = ObjectType::NPC;
	movable = true;
	solid = true;
	position = initialPosition;
	nameID = creature.getNameID();
	color = creature.getColor();
	letter = creature.getLetter();
}

NPC::NPC(GameObjects& gameObjects, std::ifstream& resource) : GameObject(gameObjects)
{
	type = ObjectType::NPC;
	load(resource);
	creature = Creature(resourceName);
}

TextPair NPC::getText()
{
	return {TextCategory::Creature, nameID};
}

void NPC::load(std::ifstream& resource)
{
	GameObject::load(resource);
	unsigned int size;
	resource.read(reinterpret_cast<char*>(&size), SIZE_INT);
	char resourceChar[size];
	resourceName = resourceChar;
}

void NPC::save(std::ofstream& resource)
{
	GameObject::save(resource);
	unsigned int size = resourceName.size();
	const char* resourceChar = resourceName.c_str();
	resource.write(reinterpret_cast<char*>(&size), SIZE_INT);
	resource.write(resourceChar, size);
}
