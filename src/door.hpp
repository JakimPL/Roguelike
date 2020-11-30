#ifndef NPC_HPP
#define NPC_HPP

#include "gameobject.hpp"

class Door : public GameObject
{
private:
	bool orientation;
	bool locked = false;
	bool open = false;

public:
	Door(GameObjects& gameObjects, Color initialColor, bool initialOrientation, Position initialPosition, Area* initialArea);

	TextPair getText() override;
};

#endif // NPC_HPP
