#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "creature.hpp"
#include "gameobject.hpp"

class Player : public GameObject
{
private:
	std::string name;

public:
	Character character;
	Creature creature;

public:
	Player(std::string initialName);
	Player(Creature initialCreature, std::string initialName);

	std::string getName() const;
};

#endif // PLAYER_HPP
