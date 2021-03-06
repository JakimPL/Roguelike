#ifndef GAME_HPP
#define GAME_HPP

#include "area.hpp"
#include "dialog.hpp"
#include "globalvariables.hpp"
#include "keyboard.hpp"
#include "message.hpp"
#include "store.hpp"
#include "text.hpp"
#include "timer.hpp"
#include "objects/npc.hpp"
#include "objects/player.hpp"

#include <SDL2/SDL_ttf.h>

enum class GUI : unsigned int {
	None,
	Inventory,
	Character,
	Dialog,
	Map,
	Store
};

class Game
{
private:
	SDL_Renderer* renderer;
	SDL_Window* window;
	TTF_Font* font;
	Keyboard keyboard;
	GUI activeTab = GUI::None;
	bool update = true;

	struct {
		SDL_Texture* guiTexture;
		SDL_Texture* messagesTexture;
		SDL_Texture* objectsTexture;
		SDL_Texture* worldTexture;
		SDL_Rect screenRectangle;
		SDL_Rect worldRectangle;
	} graphics;

	unsigned long turn = 0;
	int dialogID = 0;
	unsigned int responsePage = 0;
	int responsePosition = 0;
	unsigned int inventoryPage = 0;
	int inventoryPosition = 0;
	unsigned int storePage = 0;
	int storePosition = 0;
	int characterInfoPosition = 0;
	bool storeTab = false;
	Position mapOffset = {0, 0, 0};

	std::vector<std::string> currentDialogLineText;
	std::vector<unsigned int> possibleResponsesID;
	std::vector<std::vector<std::string>> currentResponsesTest;
	std::vector<unsigned int> responsesHeights;
	std::vector<unsigned int> responsesOffsets;
	unsigned int responsesHeight;

	std::map<std::pair<int, int>, bool> visibilityMap;
	std::vector<std::vector<bool>> seen;
public:
	Game();
	~Game();
	Text text;
	Area* currentArea;
	Store* currentStore;
	Dialog* currentDialog;
	GameObject* targetObject;
	Messages* messages;
	GameObjects gameObjects;
	GlobalState globalState;
	Player player;

	void mainLoop();
	void quit();

private:
	int clampX(int x);
	int clampY(int y);

	void calculateVisibilityMap();

	void drawFrame();
	void drawWorld();
	void drawObjects();
	void drawGUI();
	void drawCharacterInfo();
	void drawDialog();
	void drawInventory();
	void drawItemDescription(Item* item);
	void drawMap();
	void drawStore();

	void redrawWorld();
	void redrawGUI();
	void redrawMessages();

	void initializeFont();
	void initializeGraphics();
	bool isGUIactive() const;
	void openTab(GUI tab);
	void openStore(NPC* npc);
	void prepareDialog();
	void startDialog(NPC* npc);
};

#endif // GAME_HPP
