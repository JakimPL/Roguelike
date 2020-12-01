#include "door.hpp"
#include "options.hpp"

Door::Door(GameObjects& gameObjects, Color initialColor, bool initialOrientation, Position initialPosition, Area* initialArea) : GameObject(gameObjects), orientation(initialOrientation)
{
	type = ObjectType::Door;
	movable = false;
	position = initialPosition;
	color = initialColor;
	update();
	currentArea = initialArea;
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
