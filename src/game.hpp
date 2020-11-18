#ifndef GAME_HPP
#define GAME_HPP

#include "area.hpp"
#include "keyboard.hpp"
#include "message.hpp"
#include "player.hpp"
#include "text.hpp"
#include "timer.hpp"

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
	Keyboard keyboard;
	Player player;
	GUI activeTab = GUI::None;

	struct {
		SDL_Texture* worldTexture;
		SDL_Texture* guiTexture;
		SDL_Rect screenRectangle;
		SDL_Rect worldRectangle;
	} graphics;

	unsigned int inventoryPage = 0;
	int inventoryPosition = 0;
	int characterInfoPosition = 0;
	Position mapOffset = {0, 0, 0};

public:
	Game();
	Text text;
	Area currentArea;
	Messages messages;

	void mainLoop();
	void quit();

private:
	void drawFrame();
	void drawWorld();
	void drawPlayer();
	void drawGUI();
	void drawCharacterInfo();
	void drawInventory();
	void drawItemDescription(Item* item);
	void drawMap();

	void redrawWorld();
	void redrawPlayer();

	void initializeFont();
	void initializeGraphics();
	bool isGUIactive() const;
	void openTab(GUI tab);
};

#endif // GAME_HPP
