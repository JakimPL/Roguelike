#include "game.hpp"
#include "graphics.hpp"
#include "log.hpp"

using namespace Graphics;

Game::Game() : player("Liop"), currentArea("MOONDALE")
{
	initializeGraphics();
	initializeFont();
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
	drawText(renderer, font, text[ {TextCategory::Area, currentArea.getTextID()} ], COLOR_WHITE, 2 * GUI_X_OFFSET, GUI_Y_OFFSET + TILE_HEIGHT / 2, Alignment::Left);

	std::stringstream nextLevelText;
	nextLevelText << text[String::General::Next] << player.creature.getRemainingXP();
	drawText(renderer, font, nextLevelText.str(), COLOR_YELLOW, SCREEN_WIDTH / 2, GUI_Y_OFFSET + TILE_HEIGHT / 2, Alignment::Center);

	std::stringstream hpText;
	hpText << text[String::General::HP] << player.creature.getCurrentHP() << "/" << player.creature.getMaxHP();
	drawText(renderer, font, hpText.str(), COLOR_RED, SCREEN_WIDTH - 2 * GUI_X_OFFSET, GUI_Y_OFFSET + TILE_HEIGHT / 2, Alignment::Right);

	std::stringstream mpText;
	mpText << text[String::General::MP] << player.creature.getCurrentMP() << "/" << player.creature.getMaxMP();
	drawText(renderer, font, mpText.str(), COLOR_BLUE, SCREEN_WIDTH - 2 * GUI_X_OFFSET, GUI_Y_OFFSET + 3 * TILE_HEIGHT / 2, Alignment::Right);

	std::stringstream locationText;
	locationText << text[String::General::X] << playerPosition.x << text[String::General::Y] << playerPosition.y;
	drawText(renderer, font, locationText.str(), COLOR_YELLOW, SCREEN_WIDTH / 2, GUI_Y_OFFSET + 3 * TILE_HEIGHT / 2, Alignment::Center);

	std::stringstream playerInfoText;
	playerInfoText << player.getName() << text[String::General::level] << player.creature.getLevel();
	drawText(renderer, font, playerInfoText.str(), COLOR_WHITE, 2 * GUI_X_OFFSET, SCREEN_HEIGHT - GUI_Y_OFFSET - 5 * TILE_HEIGHT / 2, Alignment::Left);

	std::stringstream weaponInUseText;
	weaponInUseText << text[String::General::WeaponInUse] << text[ {TextCategory::Item, player.creature.getWeaponTextID()} ];
	drawText(renderer, font, weaponInUseText.str(), COLOR_DGREEN, 2 * GUI_X_OFFSET, SCREEN_HEIGHT - GUI_Y_OFFSET - 3 * TILE_HEIGHT / 2, Alignment::Left);

	std::stringstream mapObjectText;
	int dirX = DIR_X(playerPosition.direction);
	int dirY = DIR_Y(playerPosition.direction);
	Tile objectTile = currentArea.getTile(playerPosition.x + dirX, playerPosition.y + dirY);
	drawText(renderer, font, text[ {TextCategory::Object, objectTile.textID} ], objectTile.color, 2 * GUI_X_OFFSET, SCREEN_HEIGHT - GUI_Y_OFFSET - TILE_HEIGHT / 2, Alignment::Left);

	std::stringstream shortcutsText;
	shortcutsText << text[String::General::SHORTCUTS];
	drawText(renderer, font, shortcutsText.str(), COLOR_BROWN, SCREEN_WIDTH - 2 * GUI_X_OFFSET, SCREEN_HEIGHT - GUI_Y_OFFSET - 5 * TILE_HEIGHT / 2, Alignment::Right);

	if (isGUIactive()) {
		drawRectangle(renderer, COLOR_WHITE, TAB_X_OFFSET - 4 * SCALE, TAB_Y_OFFSET - 4 * SCALE, TAB_WIDTH + 8 * SCALE, TAB_HEIGHT + 8 * SCALE, true);
		drawRectangle(renderer, GUI_RECTANGLE_COLOR, TAB_X_OFFSET + 2 * SCALE, TAB_Y_OFFSET + 2 * SCALE, TAB_WIDTH - 4 * SCALE, TAB_HEIGHT - 4 * SCALE);
	}

	switch (activeTab) {
	case GUI::Inventory: {
		Inventory& inventory = player.creature.inventory;
		if (!inventory.isEmpty()) {
			int inventoryPage = inventoryPosition / INVENTORY_ITEMS_PER_PAGE;
			drawRectangle(renderer, GUI_INVENTORY_COLOR, TAB_X_OFFSET, TAB_Y_OFFSET + (inventoryPosition % INVENTORY_ITEMS_PER_PAGE) * TILE_HEIGHT, TAB_WIDTH / 2, TILE_HEIGHT);
			for (size_t index = 0; index < INVENTORY_ITEMS_PER_PAGE; ++index) {
				if (index + inventoryPage * INVENTORY_ITEMS_PER_PAGE >= inventory.getBackpackSize()) {
					break;
				}

				Item* item = inventory.getBackpackItem(index + inventoryPage * INVENTORY_ITEMS_PER_PAGE);
				drawText(renderer, font, text[ {TextCategory::Item, item->getTextID()} ], item->getColor(), TAB_X_OFFSET + 2 * SCALE, TAB_Y_OFFSET + (0.5f + index) * TILE_HEIGHT, Alignment::Left, Alignment::Center);
				drawItemDescription(item);
			}
		} else {
			drawText(renderer, font, text[String::General::EmptyBackpack], COLOR_BROWN, TAB_X_OFFSET + 2 * SCALE, TAB_Y_OFFSET + 0.5f * TILE_HEIGHT, Alignment::Left, Alignment::Center);
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

void Game::drawItemDescription(Item *item)
{
	int line = 0;
	const int xOffset = TAB_X_OFFSET + 3 * TAB_WIDTH / 4;
	const int yOffset = TAB_Y_OFFSET + 0.5f * TILE_HEIGHT;
	drawText(renderer, font, text[ {TextCategory::Item, item->getTextID()} ], item->getColor(), xOffset, yOffset + (TILE_HEIGHT * line++), Alignment::Center);

	std::stringstream descriptionText;
	if (item->getDescriptionID() > 0) {
		descriptionText << text[ {TextCategory::Item, item->getDescriptionID()} ];
	} else {
		switch (item->getType()) {
		case ItemType::weapon:
		case ItemType::armor:
		case ItemType::quick:
			descriptionText << text[categoryTextIDs[(size_t)(item->getCategory())]];
			break;
		default:
			descriptionText << text[typeTextIDs[(size_t)(item->getType())]];
			break;
		}
	}

	drawText(renderer, font, descriptionText.str(), item->getColor(), xOffset, yOffset + (TILE_HEIGHT * line++), Alignment::Center);

	if (item->getRequiredLevel() > 0) {
		SDL_Color sdlColor = (item->getRequiredLevel() > player.creature.getLevel() ? SDL_Color(COLOR_RED) : SDL_Color(COLOR_GREEN));
		drawText(renderer, font, STRING(item->getRequiredLevel()), sdlColor, xOffset, yOffset + (TILE_HEIGHT * line++), Alignment::Center);
	}

	if (item->getDamage() + item->getDamageDelta() > 0) {
		std::stringstream attackText, attackRateText;
		attackText << text[String::General::Damage] << item->getDamage() << "-" << item->getDamage() + item->getDamageDelta();
		attackRateText << text[String::General::AttackRate] << item->getAttackRate();
		drawText(renderer, font, attackText.str(), COLOR_BROWN, xOffset, yOffset + (TILE_HEIGHT * line++), Alignment::Center);
		drawText(renderer, font, attackRateText.str(), COLOR_BROWN, xOffset, yOffset + (TILE_HEIGHT * line++), Alignment::Center);
	}
	if (item->getDefense() + item->getDefenseRate() > 0) {
		std::stringstream defenseText, defenseRateText;
		defenseText << text[String::General::Defense] << item->getDefense();
		defenseRateText << text[String::General::DefenseRate] << item->getDefenseRate();
		drawText(renderer, font, defenseText.str(), COLOR_BROWN, xOffset, yOffset + (TILE_HEIGHT * line++), Alignment::Center);
		drawText(renderer, font, defenseRateText.str(), COLOR_BROWN, xOffset, yOffset + (TILE_HEIGHT * line++), Alignment::Center);
	}
	if (item->getDelay() > 0) {
		std::stringstream delayText;
		delayText << text[item->getType() == ItemType::weapon ? String::General::Delay : String::General::Speed] << item->getDelay();
		drawText(renderer, font, delayText.str(), COLOR_BROWN, xOffset, yOffset + (TILE_HEIGHT * line++), Alignment::Center);
	}

	for (size_t abilityIndex = 0; abilityIndex < Ability::count; ++abilityIndex) {
		Ability ability = Ability(abilityIndex);
		drawText(renderer, font, text[ {TextCategory::General, size_t(String::General::STR) + abilityIndex} ], COLOR_WHITE, TAB_X_OFFSET + TAB_WIDTH / 2 + (abilityIndex - float(Ability::count - 1) / 2) * INVENTORY_ABILITIES_DISTANCE * TILE_WIDTH, TAB_Y_OFFSET + (0.5f + INVENTORY_ITEMS_PER_PAGE) * TILE_HEIGHT, Alignment::Center);
		drawText(renderer, font, STRING(player.creature.getAbilityValue(ability)), COLOR_WHITE, TAB_X_OFFSET + TAB_WIDTH / 2 + (ability - float(Ability::count - 1) / 2) * INVENTORY_ABILITIES_DISTANCE * TILE_WIDTH, TAB_Y_OFFSET + (1.5f + INVENTORY_ITEMS_PER_PAGE) * TILE_HEIGHT, Alignment::Center);

		SDL_Color sdlColor = (item->getRequiredAbility(ability) > player.creature.getAbilityValue(ability) ? SDL_Color(COLOR_RED) : SDL_Color(COLOR_GREEN));
		drawText(renderer, font, STRING(item->getRequiredAbility(ability)), sdlColor, TAB_X_OFFSET + TAB_WIDTH / 2 + (ability - float(Ability::count - 1) / 2) * INVENTORY_ABILITIES_DISTANCE * TILE_WIDTH, TAB_Y_OFFSET + (2.5f + INVENTORY_ITEMS_PER_PAGE) * TILE_HEIGHT, Alignment::Center);
	}
}

bool Game::isGUIactive() const
{
	return (unsigned int)(activeTab) > 0;
}

void Game::openTab(GUI tab)
{
	if (activeTab == tab) {
		activeTab = GUI::None;
	} else {
		activeTab = tab;
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
				openTab(GUI::Inventory);
			} else if (keyboard.isKeyPressed(SDLK_c)) {
				openTab(GUI::Character);
			} else if (keyboard.isKeyPressed(SDLK_m)) {
				openTab(GUI::Map);
			}

			if (keyboard.isKeyPressed(SDLK_ESCAPE)) {
				if (activeTab == GUI::None) {
					quit = true;
				} else {
					activeTab = GUI::None;
				}
			}

			switch (activeTab) {
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
				if (!player.creature.inventory.isEmpty()) {
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

void Game::initializeFont()
{
	if (TTF_Init() == -1) {
		_LogError("Error initializing TTF: " << TTF_GetError());
		throw std::runtime_error("error initializing TTF");
	}

	_LogInfo("Opening font file " << PATH_FONT);
	font = TTF_OpenFont(PATH_FONT, FONT_SIZE);
	if (font == NULL) {
		_LogError("Failed to load font file!");
		throw std::runtime_error("failed to load font file");
	}
}

void Game::initializeGraphics()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		_LogError("Error initializing SDL: " << SDL_GetError());
		throw std::runtime_error("error initializing SDL");
	}

	int screenWidth = SCREEN_WIDTH;
	int screenHeight = SCREEN_HEIGHT;
	if (FULLSCREEN) {
		SDL_GetRendererOutputSize(renderer, &screenWidth, &screenHeight);
	}

	window = SDL_CreateWindow("Roguelike", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, 0);

	if (FULLSCREEN) {
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	if (renderer == NULL) {
		_LogError("Bad SDL object!");
		throw std::runtime_error("bad SDL object");
	}

	if (INVENTORY_ITEMS_PER_PAGE <= 0) {
		_LogError("Too small inventory size!");
		throw std::runtime_error("too small inventory size");
	}
}

void Game::quit()
{
	TTF_Quit();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
