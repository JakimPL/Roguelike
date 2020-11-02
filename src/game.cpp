#include "game.hpp"

using namespace Functions;

Game::Game() : player("Liop"), currentArea("MOONDALE")
{
	// init SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		_LogError("Error initializing SDL: " << SDL_GetError());
		throw std::runtime_error("error initializing SDL");
	}

	window = SDL_CreateWindow("Roguelike", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	if (renderer == NULL) {
		_LogError("Bad SDL object!");
		throw std::runtime_error("bad SDL object");
	}

	// init TTF
	if (TTF_Init() == -1) {
		_LogError("Error initializing TTF: " << TTF_GetError());
		throw std::runtime_error("error initializing TTF");
	}

	// load a font
	_LogInfo("Opening font file " << PATH_FONT);
	font = TTF_OpenFont(PATH_FONT, FONT_SIZE);
	if (font == NULL) {
		_LogError("Failed to load font file!");
		throw std::runtime_error("failed to load font file");
	}

	player.currentArea = &currentArea;
}

void Game::drawFrame()
{
	SDL_RenderClear(renderer);
	SDL_RenderSetScale(renderer, SCALE, SCALE);

	drawMap();
	drawPlayer();
	drawGUI();

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderPresent(renderer);
}

void Game::drawMap()
{
	size_t width = currentArea.getWidth();
	size_t height = currentArea.getHeight();
	Position playerPosition = player.getPosition();
	for (size_t y = 0; y < height; ++y) {
		for (size_t x = 0; x < width; ++x) {
			Tile tile = currentArea.getTile(x, y);
			drawLetter(renderer, font, tile.letter, tile.color, x - playerPosition.x + CENTER_X, y - playerPosition.y + CENTER_Y);
		}
	}
}

void Game::drawPlayer()
{
	Position playerPosition = player.getPosition();
	int dirX = DIR_X(playerPosition.direction);
	int dirY = DIR_Y(playerPosition.direction);
	drawLetter(renderer, font, TARGET_CHAR, TARGET_COLOR, CENTER_X + dirX, CENTER_Y + dirY);
	drawLetter(renderer, font, player.creature.getLetter(), player.creature.getColor(), CENTER_X, CENTER_Y);
}

void Game::drawGUI()
{
	Position playerPosition = player.getPosition();
	drawRectangle(renderer, GUI_RECTANGLE_COLOR, GUI_X_OFFSET, GUI_Y_OFFSET, SCREEN_WIDTH - 2 * GUI_X_OFFSET, TILE_HEIGHT * 3);
	drawRectangle(renderer, GUI_RECTANGLE_COLOR, GUI_X_OFFSET, SCREEN_HEIGHT - GUI_Y_OFFSET - 3 * TILE_HEIGHT, SCREEN_WIDTH - 2 * GUI_X_OFFSET, TILE_HEIGHT * 3);
	drawText(renderer, font, text[currentArea.getTextID()], COLOR_WHITE, 2 * GUI_X_OFFSET, GUI_Y_OFFSET + TILE_HEIGHT / 2, Alignment::Left);

	std::stringstream nextLevelText;
	nextLevelText << text[String::Next] << player.creature.getRemainingXP();
	drawText(renderer, font, nextLevelText.str(), COLOR_YELLOW, SCREEN_WIDTH / 2, GUI_Y_OFFSET + TILE_HEIGHT / 2, Alignment::Center);

	std::stringstream hpText;
	hpText << text[String::HP] << player.creature.getCurrentHP() << "/" << player.creature.getMaxHP();
	drawText(renderer, font, hpText.str(), COLOR_RED, SCREEN_WIDTH - 2 * GUI_X_OFFSET, GUI_Y_OFFSET + TILE_HEIGHT / 2, Alignment::Right);

	std::stringstream mpText;
	mpText << text[String::HP] << player.creature.getCurrentMP() << "/" << player.creature.getMaxMP();
	drawText(renderer, font, mpText.str(), COLOR_BLUE, SCREEN_WIDTH - 2 * GUI_X_OFFSET, GUI_Y_OFFSET + 3 * TILE_HEIGHT / 2, Alignment::Right);

	std::stringstream locationText;
	locationText << text[String::X] << playerPosition.x << text[String::Y] << playerPosition.y;
	drawText(renderer, font, locationText.str(), COLOR_YELLOW, SCREEN_WIDTH / 2, GUI_Y_OFFSET + 3 * TILE_HEIGHT / 2, Alignment::Center);

	std::stringstream playerInfoText;
	playerInfoText << player.getName() << text[String::level] << player.creature.getLevel();
	drawText(renderer, font, playerInfoText.str(), COLOR_WHITE, 2 * GUI_X_OFFSET, SCREEN_HEIGHT - GUI_Y_OFFSET - 5 * TILE_HEIGHT / 2, Alignment::Left);

	std::stringstream weaponInUseText;
	weaponInUseText << text[String::WeaponInUse] << text[player.creature.getWeaponTextID()];
	drawText(renderer, font, weaponInUseText.str(), COLOR_DGREEN, 2 * GUI_X_OFFSET, SCREEN_HEIGHT - GUI_Y_OFFSET - 3 * TILE_HEIGHT / 2, Alignment::Left);

	std::stringstream mapObjectText;
	int dirX = DIR_X(playerPosition.direction);
	int dirY = DIR_Y(playerPosition.direction);
	Tile objectTile = currentArea.getTile(playerPosition.x + dirX, playerPosition.y + dirY);
	drawText(renderer, font, text[objectTile.textID], objectTile.color, 2 * GUI_X_OFFSET, SCREEN_HEIGHT - GUI_Y_OFFSET - TILE_HEIGHT / 2, Alignment::Left);

	std::stringstream shortcutsText;
	shortcutsText << text[String::SHORTCUTS];
	drawText(renderer, font, shortcutsText.str(), COLOR_BROWN, SCREEN_WIDTH - 2 * GUI_X_OFFSET, SCREEN_HEIGHT - GUI_Y_OFFSET - 5 * TILE_HEIGHT / 2, Alignment::Right);

	if (isGUIactive()) {
		drawRectangle(renderer, COLOR_WHITE, GUI_X_OFFSET + 2 * TILE_WIDTH, GUI_Y_OFFSET + 4 * TILE_HEIGHT, SCREEN_WIDTH - 2 * GUI_X_OFFSET - 4 * TILE_WIDTH, SCREEN_HEIGHT - 2 * GUI_Y_OFFSET - 8 * TILE_HEIGHT, true);
		drawRectangle(renderer, GUI_RECTANGLE_COLOR, GUI_X_OFFSET + 2 * TILE_WIDTH + SCALE, GUI_Y_OFFSET + 4 * TILE_HEIGHT + SCALE, SCREEN_WIDTH - 2 * GUI_X_OFFSET - 4 * TILE_WIDTH - 2 * SCALE, SCREEN_HEIGHT - 2 * GUI_Y_OFFSET - 8 * TILE_HEIGHT - 2 * SCALE);
	}

	switch (activeCart) {
	case GUI::Inventory: {
		Inventory& inventory = player.creature.inventory;
		int inventoryPage = inventoryPosition / INVENTORY_ITEMS_PER_PAGE;
		drawRectangle(renderer, GUI_INVENTORY_COLOR, GUI_X_OFFSET + 2 * TILE_WIDTH + 4 * SCALE, GUI_Y_OFFSET + (4 + inventoryPosition % INVENTORY_ITEMS_PER_PAGE) * TILE_HEIGHT + 4 * SCALE, SCREEN_WIDTH - 2 * GUI_X_OFFSET - 4 * TILE_WIDTH - 8 * SCALE, TILE_HEIGHT);
		for (size_t index = 0; index < INVENTORY_ITEMS_PER_PAGE; ++index) {
			if (index + inventoryPage * INVENTORY_ITEMS_PER_PAGE >= inventory.getBackpackSize()) {
				break;
			}

			Item* item = inventory.getBackpackItem(index + inventoryPage * INVENTORY_ITEMS_PER_PAGE);
			drawText(renderer, font, text[item->getTextID()], item->getColor(), 2 * GUI_X_OFFSET + 2 * TILE_WIDTH, GUI_Y_OFFSET + (4.5f + index) * TILE_HEIGHT + 2 * SCALE, Alignment::Left, Alignment::Center);
		}

		break;
	}
	case GUI::Character:
		break;
	case GUI::Map:
		break;
	default:
		break;
	}
}

bool Game::isGUIactive() const
{
	return (unsigned int)(activeCart) > 0;
}

void Game::openCart(GUI cart)
{
	if (activeCart == cart) {
		activeCart = GUI::None;
	} else {
		activeCart = cart;
	}
}

void Game::mainLoop()
{
	bool quit = false;
	SDL_Event event;
	while (!quit) {
		timer.update();
		if (timer.frame()) {
			while (SDL_PollEvent(&event) != 0) {
				switch (event.type) {
				case SDL_KEYDOWN:
				case SDL_KEYUP: {
					keyboard.update(event);
					break;
				}
				case SDL_QUIT:
					quit = true;
					break;
				}
			}

			if (keyboard.isKeyPressed(SDLK_i)) {
				openCart(GUI::Inventory);
			} else if (keyboard.isKeyPressed(SDLK_c)) {
				openCart(GUI::Character);
			} else if (keyboard.isKeyPressed(SDLK_m)) {
				openCart(GUI::Map);
			}

			if (keyboard.isKeyPressed(SDLK_ESCAPE)) {
				if (activeCart == GUI::None) {
					quit = true;
				} else {
					activeCart = GUI::None;
				}
			}

			switch (activeCart) {
			case GUI::None:
				for (unsigned int dir = 0; dir < COUNT; ++dir) {
					Direction direction = Direction(dir);
					if (keyboard.isKey(keyDirection[direction])) {
						if (keyboard.getKeyState(SDLK_LSHIFT)) {
							player.setDirection(direction);
						} else {
							player.move(direction);
						}
						break;
					}
				}

				break;
			case GUI::Inventory:
				if (keyboard.isKey(SDLK_UP) or keyboard.isKey(SDLK_KP_8)) {
					inventoryPosition = std::max(0, inventoryPosition - 1);
				}
				if (keyboard.isKey(SDLK_DOWN) or keyboard.isKey(SDLK_KP_2)) {
					inventoryPosition = std::min((int)(player.creature.inventory.getBackpackSize()) - 1, inventoryPosition + 1);
				}
				if (keyboard.isKey(SDLK_LEFT) or keyboard.isKey(SDLK_KP_4) or keyboard.isKey(SDLK_PAGEUP)) {
					inventoryPosition = std::max(0, inventoryPosition - INVENTORY_ITEMS_PER_PAGE);
				}
				if (keyboard.isKey(SDLK_RIGHT) or keyboard.isKey(SDLK_KP_6) or keyboard.isKey(SDLK_PAGEDOWN)) {
					inventoryPosition = std::min((int)(player.creature.inventory.getBackpackSize()) - 1, inventoryPosition + INVENTORY_ITEMS_PER_PAGE);
				}
				break;
			case GUI::Character:
				break;
			case GUI::Map:
				break;
			}

			keyboard.step();
			player.step();
			drawFrame();
		}
	}
}

void Game::quit()
{
	TTF_Quit();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
