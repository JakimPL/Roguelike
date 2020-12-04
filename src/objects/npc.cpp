#include "npc.hpp"
#include "src/functions.hpp"

NPC::NPC(GameObjects& gameObjects, std::string initialCreatureName, std::string initialStoreName, Position initialPosition) : GameObject(gameObjects), creatureName(initialCreatureName), storeName(initialStoreName), creature(creatureName)
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
	creature = Creature(creatureName);
	store = Store(storeName);
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
	creatureName = Functions::readString(resource, size);
	resource.read(reinterpret_cast<char*>(&size), SIZE_INT);
	storeName = Functions::readString(resource, size);
}

void NPC::save(std::ofstream& resource)
{
	GameObject::save(resource);
	unsigned int size = creatureName.size();
	resource.write(reinterpret_cast<char*>(&size), SIZE_INT);
	resource.write(creatureName.c_str(), size);
	size = storeName.size();
	resource.write(reinterpret_cast<char*>(&size), SIZE_INT);
	resource.write(storeName.c_str(), size);
}
