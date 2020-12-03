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
	NPC(GameObjects& gameObjects, Creature initialCreature, Position initialPosition);
	NPC(GameObjects& gameObjects, std::ifstream& resource);

	TextPair getText() override;
	void load(std::ifstream& resource) override;
	void save(std::ofstream& resource) override;
};

#endif // NPC_HPP
