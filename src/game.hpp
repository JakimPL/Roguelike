#ifndef GAME_HPP
#define GAME_HPP

#include "player.hpp"
#include "text.hpp"
#include <queue>

class Game
{
private:
	SDL_Renderer* renderer;
	SDL_Window* window;
	TTF_Font* font;
	Text text;
public:
	Player player;
	Area currentArea;

public:
	Game();

	void drawFrame();
	void drawMap();
	void drawPlayer();
	void drawGUI();
	void mainLoop();
	void quit();
};

#endif // GAME_HPP
