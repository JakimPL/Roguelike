#include "npc.hpp"
#include "src/functions.hpp"

NPC::NPC(GameObjects& gameObjects, std::string initialCreatureName, std::string initialDialogName, std::string initialStoreName, Position initialPosition, Allegiance initialAllegiance) : GameObject(gameObjects), creatureName(initialCreatureName), dialogName(initialDialogName), storeName(initialStoreName), allegiance(initialAllegiance), creature(creatureName), dialog(dialogName)
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
	if (!dialogName.empty()) {
		dialog = Dialog(dialogName);
	}
	if (!storeName.empty()) {
		store = Store(storeName);
	}
}

Allegiance NPC::getAllegiance() const
{
	return allegiance;
}

std::string NPC::getCreatureResourceName() const
{
	return creatureName;
}

std::string NPC::getDialogResourceName() const
{
	return dialogName;
}

std::string NPC::getStoreResourceName() const
{
	return storeName;
}

void NPC::setAllegiance(Allegiance value)
{
	allegiance = value;
}

void NPC::setCreatureResourceName(const std::string name)
{
	creatureName = name;
	creature = Creature(name);
	nameID = creature.getNameID();
	color = creature.getColor();
	letter = creature.getLetter();
}

void NPC::setDialogResourceName(const std::string name)
{
	dialogName = name;
}

void NPC::setStoreResourceName(const std::string name)
{
	storeName = name;
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

	resource.read(reinterpret_cast<char*>(&allegiance), SIZE_CHAR);
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

	resource.write(reinterpret_cast<char*>(&allegiance), SIZE_CHAR);
}
