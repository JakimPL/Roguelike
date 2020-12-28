#include "player.hpp"
#include "src/options.hpp"
#include "src/random.hpp"

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
	delay = (currentItem == nullptr ? FIST_DELAY : currentItem->getDelay());

	int damage = creature.getDamageMax();
	npc->creature.takeHP(damage);

	if (npc->creature.getHPCurrent() <= 0) {
		creature.addXP(npc->creature.getXPValue() * round(0.8f + 0.4f * Random::random()));
		remove(npc);
	}

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
