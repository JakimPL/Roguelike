#include "gameobject.hpp"
#include "constants.hpp"
#include "log.hpp"
#include "options.hpp"

GameObject::GameObject(GameObjects& gameObjects) : objects(gameObjects)
{
	gameObjects.push_back(this);
}

GameObject::~GameObject()
{

}

bool GameObject::isPositionFree(int x, int y) const
{
	return isPositionFree({x, y});
}

bool GameObject::isPositionFree(Position position) const
{
	if (!currentArea->isTileOutside(position) and currentArea->getTile(position).obstacle) {
		return false;
	}

	for (GameObject* object : objects) {
		if (object->solid and object->position == position) {
			return false;
		}
	}

	return true;
}

GameObjects GameObject::isPositionTaken(int x, int y) const
{
	return isPositionTaken({x, y});
}

GameObjects GameObject::isPositionTaken(Position position) const
{
	GameObjects objectsAtPosition;
	for (GameObject* object : objects) {
		if (object->position == position) {
			objectsAtPosition.push_back(object);
		}
	}

	return objectsAtPosition;
}

unsigned int GameObject::getNameID() const
{
	return nameID;
}

Color GameObject::getColor() const
{
	return color;
}

char GameObject::getLetter() const
{
	return letter;
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
		if (isPositionFree(targetPosition)) {
			if (delay == 0) {
				position.moveInDirection();
				delay = options.game.delay;
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

TextPair GameObject::getText()
{
	return {TextCategory::General, 0};
}

bool GameObject::load(std::ifstream& resource)
{
	resource.read(reinterpret_cast<char*>(&nameID), SIZE_INT);
	resource.read(reinterpret_cast<char*>(&delay), SIZE_INT);
	resource.read(reinterpret_cast<char*>(&movable), SIZE_CHAR);
	resource.read(reinterpret_cast<char*>(&movable), SIZE_CHAR);
	resource.read(reinterpret_cast<char*>(&position), SIZE_POSITION);
	return true;
}

bool GameObject::save(std::ofstream& resource)
{
	resource.write(reinterpret_cast<char*>(&nameID), SIZE_INT);
	resource.write(reinterpret_cast<char*>(&delay), SIZE_INT);
	resource.write(reinterpret_cast<char*>(&movable), SIZE_CHAR);
	resource.write(reinterpret_cast<char*>(&movable), SIZE_CHAR);
	resource.write(reinterpret_cast<char*>(&position), SIZE_POSITION);
	return true;
}

void GameObjects::deleteObject(GameObject* object)
{
	std::vector<GameObject*>::iterator position = std::find(begin(), end(), object);
	if (position != end()) {
		erase(position);
		delete object;
	}
}
