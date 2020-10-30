#include "main.hpp"
#include "src/constants.hpp"
#include "src/game.hpp"
#include "src/log.hpp"

#include <sstream>

int main()
{
	_LogNone("Game starts");

	// initialize game objects
	Game game;
	game.mainLoop();
	game.quit();

	_LogNone("Game ends");
	return OK;
}
