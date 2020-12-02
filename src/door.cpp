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

TextPair Door::getText()
{
	return Text::makeTextPair(String::Door);
}

void Door::load(std::ifstream& resource)
{
	resource.read(reinterpret_cast<char*>(&orientation), SIZE_CHAR);
	resource.read(reinterpret_cast<char*>(&locked), SIZE_CHAR);
	resource.read(reinterpret_cast<char*>(&open), SIZE_CHAR);
}

void Door::save(std::ofstream& resource)
{
	resource.write(reinterpret_cast<char*>(&orientation), SIZE_CHAR);
	resource.write(reinterpret_cast<char*>(&locked), SIZE_CHAR);
	resource.write(reinterpret_cast<char*>(&open), SIZE_CHAR);
}
