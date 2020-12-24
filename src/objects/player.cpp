#include "player.hpp"

Player::Player(GameObjects& gameObjects, Creature initialCreature, Position initialPosition, std::string initialName) : GameObject(gameObjects), creature(initialCreature)
{
	type = ObjectType::Player;
	name = initialName;
	movable = true;
	position = initialPosition;
	color = creature.getColor();
	letter = creature.getLetter();
}

std::string Player::getName() const
{
	return name;
}

int Player::hit(NPC* npc)
{
	Item* currentItem = creature.inventory.getStackItem(ItemType::weapon);
	delay = (currentItem == nullptr ? 10 : currentItem->getDelay());

	int damage = creature.getDamageMax();
	npc->creature.takeHP(damage);

	return damage;
}

TextPair Player::getText()
{
	return Text::makeTextPair(s_empty);
}

void Player::load(std::ifstream& resource)
{
	creature.load(resource);
}

void Player::save(std::ofstream& resource)
{
	creature.save(resource);
}
