#include "door.hpp"
#include "options.hpp"

Door::Door(GameObjects& gameObjects, Color initialColor, bool initialOrientation, Position initialPosition, Area* initialArea) : GameObject(gameObjects), orientation(initialOrientation)
{
	type = ObjectType::Door;
	movable = false;
	solid = !open;
	position = initialPosition;
	color = initialColor;
	letter = ((!open) != (!orientation) ? options.game.doorHorizontalLetter : options.game.doorVerticalLetter);
	currentArea = initialArea;
}

TextPair Door::getText()
{
	return Text::makeTextPair(String::Door);
}
