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
	Timer timer;
	Keyboard keyboard;
public:
	Player player;
	Area currentArea;
	GUI activeCart = GUI::None;
	unsigned int inventoryPage = 0;
	int inventoryPosition = 0;
public:
	Game();

	void drawFrame();
	void drawMap();
	void drawPlayer();
	void drawGUI();

	bool isGUIactive() const;
	void openCart(GUI cart);

	void mainLoop();
	void quit();
};

#endif // GAME_HPP
