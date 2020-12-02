#include "npc.hpp"

NPC::NPC(GameObjects& gameObjects, Creature initialCreature, Position initialPosition) : GameObject(gameObjects), creature(initialCreature)
{
	type = ObjectType::NPC;
	movable = true;
	solid = true;
	position = initialPosition;
	nameID = creature.getNameID();
	color = creature.getColor();
	letter = creature.getLetter();
}

TextPair NPC::getText()
{
	return {TextCategory::Creature, nameID};
}

void NPC::load(std::ifstream& resource)
{
	creature.load(resource);
}

void NPC::save(std::ofstream& resource)
{
	creature.save(resource);
}
