#include "gameobject.hpp"
#include "src/constants.hpp"
#include "src/options.hpp"

GameObject::GameObject(GameObjects& gameObjects) : objects(gameObjects)
{
	gameObjects.push_back(this);
}

GameObject::~GameObject()
{

}

bool GameObject::exists(GameObject* object)
{
	return std::find(objects.begin(), objects.end(), object) != objects.end();
}

void GameObject::remove(GameObject* object)
{
	objects.erase(std::find(objects.begin(), objects.end(), object));
	delete object;
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

bool GameObject::isSolid() const
{
	return solid;
}

void GameObject::setNameID(unsigned int index)
{
	nameID = index;
}

void GameObject::setColor(Color newColor)
{
	color = newColor;
}

void GameObject::setLetter(char newLetter)
{
	letter = newLetter;
}

void GameObject::setDirection(Direction direction)
{
	position.direction = direction;
}

void GameObject::move(Direction direction)
{
	if (movable) {
		position.direction = direction;

		if (delay == 0) {
			position.moveInDirection();
			delay = options.game.delay;
		}
	}
}

void GameObject::move()
{
	move(Direction(position.direction));
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

void GameObject::load(std::ifstream& resource)
{
	resource.read(reinterpret_cast<char*>(&nameID), SIZE_INT);
	resource.read(reinterpret_cast<char*>(&delay), SIZE_INT);
	resource.read(reinterpret_cast<char*>(&movable), SIZE_CHAR);
	resource.read(reinterpret_cast<char*>(&solid), SIZE_CHAR);
	resource.read(reinterpret_cast<char*>(&position), SIZE_POSITION);
	resource.read(reinterpret_cast<char*>(&color), SIZE_COLOR);
	resource.read(reinterpret_cast<char*>(&letter), SIZE_CHAR);
}

void GameObject::save(std::ofstream& resource)
{
	resource.write(reinterpret_cast<char*>(&type), SIZE_CHAR);
	resource.write(reinterpret_cast<char*>(&nameID), SIZE_INT);
	resource.write(reinterpret_cast<char*>(&delay), SIZE_INT);
	resource.write(reinterpret_cast<char*>(&movable), SIZE_CHAR);
	resource.write(reinterpret_cast<char*>(&solid), SIZE_CHAR);
	resource.write(reinterpret_cast<char*>(&position), SIZE_POSITION);
	resource.write(reinterpret_cast<char*>(&color), SIZE_COLOR);
	resource.write(reinterpret_cast<char*>(&letter), SIZE_CHAR);
}

void GameObjects::deleteObject(GameObject* object)
{
	std::vector<GameObject*>::iterator position = std::find(begin(), end(), object);
	if (position != end()) {
		erase(position);
		delete object;
	}
}

void GameObjects::deleteObjects()
{
	for (GameObject* object : *this) {
		if (object->type != ObjectType::Player) {
			deleteObject(object);
		}
	}
}
