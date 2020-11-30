#ifndef NPC_HPP
#define NPC_HPP

#include "creature.hpp"
#include "gameobject.hpp"

class NPC : public GameObject
{
private:

public:
	Creature creature;

public:
	NPC(GameObjects& gameObjects, Creature initialCreature, Position initialPosition, Area* initialArea);

	TextPair getText() override;
};

#endif // NPC_HPP
