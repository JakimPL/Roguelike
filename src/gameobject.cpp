#include "gameobject.hpp"
#include "constants.hpp"

GameObject::GameObject()
{
	gameObjects.push_back(this);
}

Position GameObject::getPosition() const
{
	return position;
}

void GameObject::move(Direction direction)
{
	if (movable) {
		position.direction = direction;
		Position targetPosition = position + 1;
		if (!currentArea->getTile(targetPosition).obstacle or currentArea->isTileOutside(targetPosition)) {
			if (delay == 0) {
				position.moveInDirection();
				delay = DELAY;
			}
		}
	}
}

void GameObject::setDirection(Direction direction)
{
	position.direction = direction;
}

void GameObject::step()
{
	if (delay > 0) {
		delay--;
	}
}
