#include "player.hpp"
#include "src/log.hpp"
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

	int damage = 0;
	if (Random::random(npc->creature.getDefenseRate()) + 0.5f * npc->creature.getDefenseRate() < Random::random(5 + creature.getAttackRate()) + 0.58f * creature.getAttackRate()) {
		damage = std::max(0.0, round(Random::random(creature.getDamageMin(), creature.getDamageMax())) - 0.2f * npc->creature.getDefense());

		if (damage > 0) {
			npc->creature.takeHP(damage);
			if (npc->creature.getHPCurrent() <= 0) {
				int xp = round((0.8 + 0.4 * Random::random()) * npc->creature.getXPValue());
				creature.addXP(xp);
				remove(npc);
			}
		}
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
