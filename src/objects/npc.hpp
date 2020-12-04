#ifndef NPC_HPP
#define NPC_HPP

#include "gameobject.hpp"
#include "src/creature.hpp"
#include "src/store.hpp"

class NPC : public GameObject
{
private:

public:
	std::string creatureName;
	std::string storeName;
	Creature creature;
	Store store;

public:
	NPC(GameObjects& gameObjects, std::string initialCreatureName, std::string initialStoreName, Position initialPosition);
	NPC(GameObjects& gameObjects, std::ifstream& resource);

	TextPair getText() override;
	void load(std::ifstream& resource) override;
	void save(std::ofstream& resource) override;
};

#endif // NPC_HPP
