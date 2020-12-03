#include "door.hpp"
#include "options.hpp"

Door::Door(GameObjects& gameObjects, Color initialColor, bool initialOrientation, bool initialLocked, Position initialPosition) : GameObject(gameObjects), orientation(initialOrientation), locked(initialLocked)
{
	type = ObjectType::Door;
	movable = false;
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
	solid = !open;
	letter = ((!open) != (!orientation) ? options.game.doorHorizontalLetter : options.game.doorVerticalLetter);
}

bool Door::getOrientation() const
{
	return orientation;
}

void Door::setOrientation(bool newOrientation)
{
	orientation = newOrientation;
}

TextPair Door::getText()
{
	return Text::makeTextPair(String::Door);
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
