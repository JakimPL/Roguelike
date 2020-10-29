#ifndef GAME_HPP
#define GAME_HPP

#include "player.hpp"
#include <queue>

class Game
{
public:
	Player player;
	Area currentArea;

public:
	Game();
};

#endif // GAME_HPP
