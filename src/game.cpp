#include "game.hpp"

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
			Functions::drawLetter(renderer, font, tile.letter, tile.color, x - playerPosition.x + CENTER_X, y - playerPosition.y + CENTER_Y);
		}
	}
}

void Game::drawPlayer()
{
	Position playerPosition = player.getPosition();
	int dirX = DIR_X(playerPosition.direction);
	int dirY = DIR_Y(playerPosition.direction);
	Functions::drawLetter(renderer, font, TARGET_CHAR, TARGET_COLOR, CENTER_X + dirX, CENTER_Y + dirY);
	Functions::drawLetter(renderer, font, player.creature.getLetter(), player.creature.getColor(), CENTER_X, CENTER_Y);
}

void Game::drawGUI()
{
	Position playerPosition = player.getPosition();
	Functions::drawRectangle(renderer, GUI_RECTANGLE_COLOR, GUI_X_OFFSET, GUI_Y_OFFSET, SCREEN_WIDTH - 2 * GUI_X_OFFSET, TILE_HEIGHT * 3);
	Functions::drawRectangle(renderer, GUI_RECTANGLE_COLOR, GUI_X_OFFSET, SCREEN_HEIGHT - GUI_Y_OFFSET - 3 * TILE_HEIGHT, SCREEN_WIDTH - 2 * GUI_X_OFFSET, TILE_HEIGHT * 3);
	Functions::drawText(renderer, font, text[currentArea.getTextID()], COLOR_WHITE, 2 * GUI_X_OFFSET, GUI_Y_OFFSET + TILE_HEIGHT / 2, Alignment::Left);

	std::stringstream nextLevelText;
	nextLevelText << text[String::Next] << player.creature.getRemainingXP();
	Functions::drawText(renderer, font, nextLevelText.str(), COLOR_YELLOW, SCREEN_WIDTH / 2, GUI_Y_OFFSET + TILE_HEIGHT / 2, Alignment::Center);

	std::stringstream hpText;
	hpText << text[String::HP] << player.creature.getCurrentHP() << "/" << player.creature.getMaxHP();
	Functions::drawText(renderer, font, hpText.str(), COLOR_RED, SCREEN_WIDTH - 2 * GUI_X_OFFSET, GUI_Y_OFFSET + TILE_HEIGHT / 2, Alignment::Right);

	std::stringstream mpText;
	mpText << text[String::HP] << player.creature.getCurrentMP() << "/" << player.creature.getMaxMP();
	Functions::drawText(renderer, font, mpText.str(), COLOR_BLUE, SCREEN_WIDTH - 2 * GUI_X_OFFSET, GUI_Y_OFFSET + 3 * TILE_HEIGHT / 2, Alignment::Right);

	std::stringstream locationText;
	locationText << text[String::X] << playerPosition.x << text[String::Y] << playerPosition.y;
	Functions::drawText(renderer, font, locationText.str(), COLOR_YELLOW, SCREEN_WIDTH / 2, GUI_Y_OFFSET + 3 * TILE_HEIGHT / 2, Alignment::Center);

	std::stringstream playerInfoText;
	playerInfoText << player.getName() << text[String::level] << player.creature.getLevel();
	Functions::drawText(renderer, font, playerInfoText.str(), COLOR_WHITE, 2 * GUI_X_OFFSET, SCREEN_HEIGHT - GUI_Y_OFFSET - 5 * TILE_HEIGHT / 2, Alignment::Left);

	std::stringstream weaponInUseText;
	weaponInUseText << text[String::WeaponInUse] << text[player.creature.getWeaponTextID()];
	Functions::drawText(renderer, font, weaponInUseText.str(), COLOR_DGREEN, 2 * GUI_X_OFFSET, SCREEN_HEIGHT - GUI_Y_OFFSET - 3 * TILE_HEIGHT / 2, Alignment::Left);

	std::stringstream mapObjectText;
	int dirX = DIR_X(playerPosition.direction);
	int dirY = DIR_Y(playerPosition.direction);
	Tile objectTile = currentArea.getTile(playerPosition.x + dirX, playerPosition.y + dirY);
	Functions::drawText(renderer, font, text[objectTile.textID], objectTile.color, 2 * GUI_X_OFFSET, SCREEN_HEIGHT - GUI_Y_OFFSET - TILE_HEIGHT / 2, Alignment::Left);

	std::stringstream shortcutsText;
	shortcutsText << text[String::SHORTCUTS];
	Functions::drawText(renderer, font, shortcutsText.str(), COLOR_BROWN, SCREEN_WIDTH - 2 * GUI_X_OFFSET, SCREEN_HEIGHT - GUI_Y_OFFSET - 5 * TILE_HEIGHT / 2, Alignment::Right);

	if (isGUIactive()) {
		Functions::drawRectangle(renderer, COLOR_WHITE, GUI_X_OFFSET + 2 * TILE_WIDTH, GUI_Y_OFFSET + 4 * TILE_HEIGHT, SCREEN_WIDTH - 2 * GUI_X_OFFSET - 4 * TILE_WIDTH, SCREEN_HEIGHT - 2 * GUI_Y_OFFSET - 8 * TILE_HEIGHT, true);
		Functions::drawRectangle(renderer, GUI_RECTANGLE_COLOR, GUI_X_OFFSET + 2 * TILE_WIDTH + SCALE, GUI_Y_OFFSET + 4 * TILE_HEIGHT + SCALE, SCREEN_WIDTH - 2 * GUI_X_OFFSET - 4 * TILE_WIDTH - 2 * SCALE, SCREEN_HEIGHT - 2 * GUI_Y_OFFSET - 8 * TILE_HEIGHT - 2 * SCALE);
	}

	switch (activeGUI) {
	case GUI::Inventory: {
		Inventory& inventory = player.creature.inventory;
		for (size_t index = 0; index < INVENTORY_ITEMS_PER_PAGE; ++index) {
			if (index >= inventory.getBackpackSize()) {
				break;
			}

			Item* item = inventory.getBackpackItem(index);
			Functions::drawText(renderer, font, text[item->getTextID()], item->getColor(), 2 * GUI_X_OFFSET + 2 * TILE_WIDTH, GUI_Y_OFFSET + (4 + index) * TILE_HEIGHT, Alignment::Left, Alignment::Top);
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
	return (unsigned int)(activeGUI) > 0;
}

void Game::mainLoop()
{
	bool change = true;
	bool quit = false;
	SDL_Event event;
	while (!quit) {
		while (SDL_PollEvent(&event) != 0) {
			change = true;
			switch (event.type) {
			case SDL_KEYDOWN: {
				switch (event.key.keysym.sym) {
				case SDLK_KP_8:
					player.move(NORTH);
					break;
				case SDLK_KP_9:
					player.move(NORTHEAST);
					break;
				case SDLK_KP_6:
					player.move(EAST);
					break;
				case SDLK_KP_3:
					player.move(SOUTHEAST);
					break;
				case SDLK_KP_2:
					player.move(SOUTH);
					break;
				case SDLK_KP_1:
					player.move(SOUTHWEST);
					break;
				case SDLK_KP_4:
					player.move(WEST);
					break;
				case SDLK_KP_7:
					player.move(NORTHWEST);
					break;
				case SDLK_ESCAPE:
					quit = true;
					break;
				}
				break;
			}
			case SDL_QUIT:
				quit = true;
				break;
			}
		}

		if (change) {
			drawFrame();
			change = false;
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
