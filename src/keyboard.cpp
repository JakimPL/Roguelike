#include "keyboard.hpp"

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
