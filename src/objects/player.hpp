#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "gameobject.hpp"
#include "src/creature.hpp"

class Player : public GameObject
{
private:
	std::string name;

public:
	Creature creature;

public:
	Player(GameObjects& gameObjects, Creature initialCreature, Position initialPosition, std::string initialName);

	std::string getName() const;

	TextPair getText() override;
	void load(std::ifstream& resource) override;
	void save(std::ofstream& resource) override;
};

#endif // PLAYER_HPP
