#ifndef NPC_HPP
#define NPC_HPP

#include "gameobject.hpp"
#include "src/creature.hpp"
#include "src/dialog.hpp"
#include "src/store.hpp"

class NPC : public GameObject
{
private:
	std::string creatureName;
	std::string dialogName;
	std::string storeName;
	Allegiance allegiance;

public:
	Creature creature;
	Dialog dialog;
	Store store;

public:
	NPC(GameObjects& gameObjects, std::string initialCreatureName, std::string initialDialogName, std::string initialStoreName, Position initialPosition, Allegiance initialAllegiance);
	NPC(GameObjects& gameObjects, std::ifstream& resource);

	Allegiance getAllegiance() const;

	TextPair getText() override;
	void load(std::ifstream& resource) override;
	void save(std::ofstream& resource) override;
};

#endif // NPC_HPP
