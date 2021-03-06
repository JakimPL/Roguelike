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
	Door(GameObjects& gameObjects, Color initialColor, bool initialOrientation, bool initialLocked, Position initialPosition);
	Door(GameObjects& gameObjects, std::ifstream& resource);

	bool toggle();
	void update();

	bool isLocked() const;
	bool isOpen() const;
	bool getOrientation() const;

	void setOrientation(bool newOrientation);
	void setLocked(bool isLocked = true);
	void setOpen(bool isOpen = true);

	TextPair getText() override;
	void load(std::ifstream& resource) override;
	void save(std::ofstream& resource) override;
};

#endif // NPC_HPP
