#ifndef GAME_HPP
#define GAME_HPP

#include "area.hpp"
#include "player.hpp"
#include "text.hpp"
#include <queue>

enum class GUI : unsigned int {
	None,
	Inventory,
	Character,
	Map
};

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
	GUI activeGUI = GUI::Inventory;
public:
	Game();

	void drawFrame();
	void drawMap();
	void drawPlayer();
	void drawGUI();

	bool isGUIactive() const;
	void mainLoop();
	void quit();
};

#endif // GAME_HPP
