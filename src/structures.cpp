#include "structures.hpp"

void Position::moveInDirection(int length, Direction chosenDirection)
{
	int dir = (chosenDirection == COUNT ? Direction(direction) : chosenDirection);
	x += length * DIR_X(dir);
	y += length * DIR_Y(dir);
}

const Position Position::operator +(int length) const
{
	Position newPosition(*this);
	newPosition.moveInDirection(length);
	return newPosition;
}

bool Character::saveToFile(const std::string&)
{
	return false;
}

bool Timer::frame()
{
	if (delta > 1000 / fps) {
		end = start;
		return true;
	}

	return false;
}

void Timer::update()
{
	start = SDL_GetTicks();
	delta = start - end;
}

bool Keyboard::getKeyState(SDL_Keycode keyCode)
{
	return keyState[keyCode];
}

bool Keyboard::isKey(SDL_Keycode keyCode)
{
	if (keyInputDelay == 0) {
		if (keyState[keyCode]) {
			keyInputDelay = KEY_INPUT_DELAY;
			return true;
		} else {
			return false;
		}
	} else {
		return false;
	}
}

bool Keyboard::isKeyPressed(SDL_Keycode keyCode)
{
	if (keyState[keyCode]) {
		if (!keyPressed[keyCode]) {
			keyPressed[keyCode] = true;
			return true;
		}
	} else {
		keyPressed[keyCode] = false;
	}

	return false;
}

void Keyboard::step()
{
	if (keyInputDelay > 0) {
		keyInputDelay--;
	}
}

void Keyboard::update(SDL_Event& event)
{
	keyState[event.key.keysym.sym] = event.key.state;
}
