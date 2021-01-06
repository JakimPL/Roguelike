#include "sign.hpp"

Sign::Sign(GameObjects& gameObjects, Color initialColor, Position initialPosition, char initialLetter, unsigned int initialNameID) : GameObject(gameObjects)
{
	type = ObjectType::Sign;
	movable = false;
	solid = true;
	transparent = false;
	letter = initialLetter;
	position = initialPosition;
	color = initialColor;
	nameID = initialNameID;
}

Sign::Sign(GameObjects& gameObjects, std::ifstream& resource) : GameObject(gameObjects)
{
	type = ObjectType::Sign;
	transparent = false;
	load(resource);
}

TextPair Sign::getText()
{
	return Text::makeTextPair(s_Sign);
}
