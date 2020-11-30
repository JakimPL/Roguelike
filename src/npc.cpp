#include "npc.hpp"

NPC::NPC(GameObjects& gameObjects, Creature initialCreature, Position initialPosition, Area* initialArea) : GameObject(gameObjects), creature(initialCreature)
{
	type = ObjectType::NPC;
	movable = true;
	solid = true;
	position = initialPosition;
	nameID = creature.getNameID();
	color = creature.getColor();
	letter = creature.getLetter();
	currentArea = initialArea;
}

TextPair NPC::getText()
{
	return {TextCategory::Creature, nameID};
}
