#include "door.hpp"
#include "src/options.hpp"

Door::Door(GameObjects& gameObjects, Color initialColor, bool initialOrientation, bool initialLocked, Position initialPosition) : GameObject(gameObjects), orientation(initialOrientation), locked(initialLocked)
{
	type = ObjectType::Door;
	position = initialPosition;
	color = initialColor;
	update();
}

Door::Door(GameObjects& gameObjects, std::ifstream& resource) : GameObject(gameObjects)
{
	type = ObjectType::Door;
	load(resource);
	update();
}

bool Door::toggle()
{
	if (!locked) {
		open = !open;
		update();
	}

	return locked;
}

void Door::update()
{
	transparent = open;
	solid = !open;
	letter = ((!open) != (!orientation) ? options.game.doorHorizontalLetter : options.game.doorVerticalLetter);
}

bool Door::isLocked() const
{
	return locked;
}

bool Door::isOpen() const
{
	return open;
}

bool Door::getOrientation() const
{
	return orientation;
}

void Door::setOrientation(bool newOrientation)
{
	orientation = newOrientation;
	update();
}

void Door::setLocked(bool isLocked)
{
	locked = isLocked;
}

void Door::setOpen(bool isOpen)
{
	open = isOpen;
	update();
}

TextPair Door::getText()
{
	return Text::makeTextPair(s_Door);
}

void Door::load(std::ifstream& resource)
{
	GameObject::load(resource);
	resource.read(reinterpret_cast<char*>(&orientation), SIZE_CHAR);
	resource.read(reinterpret_cast<char*>(&locked), SIZE_CHAR);
	resource.read(reinterpret_cast<char*>(&open), SIZE_CHAR);
}

void Door::save(std::ofstream& resource)
{
	GameObject::save(resource);
	resource.write(reinterpret_cast<char*>(&orientation), SIZE_CHAR);
	resource.write(reinterpret_cast<char*>(&locked), SIZE_CHAR);
	resource.write(reinterpret_cast<char*>(&open), SIZE_CHAR);
}
