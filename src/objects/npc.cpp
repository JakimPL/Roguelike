#include "npc.hpp"
#include "src/functions.hpp"

NPC::NPC(GameObjects& gameObjects, std::string initialCreatureName, std::string initialDialogName, std::string initialStoreName, Position initialPosition) : GameObject(gameObjects), creatureName(initialCreatureName), dialogName(initialDialogName), storeName(initialStoreName), creature(creatureName), dialog(dialogName)
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
	movable = true;
	solid = true;
	load(resource);
	creature = Creature(creatureName);
	dialog = Dialog(dialogName);
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
	dialogName = Functions::readString(resource, size);
	resource.read(reinterpret_cast<char*>(&size), SIZE_INT);
	storeName = Functions::readString(resource, size);
}

void NPC::save(std::ofstream& resource)
{
	GameObject::save(resource);
	unsigned int size = creatureName.size();
	resource.write(reinterpret_cast<char*>(&size), SIZE_INT);
	resource.write(creatureName.c_str(), size);
	size = dialogName.size();
	resource.write(reinterpret_cast<char*>(&size), SIZE_INT);
	resource.write(dialogName.c_str(), size);
	size = storeName.size();
	resource.write(reinterpret_cast<char*>(&size), SIZE_INT);
	resource.write(storeName.c_str(), size);
}
