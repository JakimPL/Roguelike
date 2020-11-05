#ifndef GAME_HPP
#define GAME_HPP

#include "area.hpp"
#include "keyboard.hpp"
#include "player.hpp"
#include "text.hpp"
#include "timer.hpp"

#include <queue>
#include <SDL2/SDL_ttf.h>

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
	GUI activeTab = GUI::None;
	unsigned int inventoryPage = 0;
	int inventoryPosition = 0;
	int characterInfoPosition = 0;
public:
	Game();

	void drawFrame();
	void drawWorld();
	void drawPlayer();
	void drawGUI();
	void drawCharacterInfo();
	void drawInventory();
	void drawItemDescription(Item* item);
	void drawMap();

	bool isGUIactive() const;
	void openTab(GUI tab);

	void mainLoop();

	void initializeFont();
	void initializeGraphics();
	void quit();
};

#endif // GAME_HPP
