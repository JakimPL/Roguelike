#ifndef DOOR_HPP
#define DOOR_HPP

enum DoorOrientation {
	Vertical,
	Horizontal
};

#include "gameobject.hpp"

class Door : public GameObject
{
private:
	bool orientation = Horizontal;
	bool locked = false;
	bool open = false;

public:
	Door(GameObjects& gameObjects, Color initialColor, bool initialOrientation, Position initialPosition, Area* initialArea);
	bool toggle();
	void update();

	TextPair getText() override;
};

#endif // NPC_HPP
