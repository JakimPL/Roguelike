#include "game.hpp"
#include "door.hpp"
#include "functions.hpp"
#include "graphics.hpp"
#include "itemobject.hpp"
#include "log.hpp"
#include "message.hpp"
#include "npc.hpp"
#include "options.hpp"

#include <iomanip>

using namespace Graphics;

Game::Game() : player(gameObjects, Creature(),
{
	30, 30, Direction::SOUTH
}, "Liop", nullptr)
{
	currentArea = new Area("MOONDALE");
	player.currentArea = currentArea;

	initializeGraphics();
	initializeFont();

	messages = new Messages(renderer, graphics.messagesTexture, font);
	new ItemObject(gameObjects, Item("DAGGER"), {10, 10});
	new NPC(gameObjects, Creature("JULIAN"), {13, 10}, currentArea);
	new Door(gameObjects, {128, 128, 128}, false, true, {15, 15}, currentArea);
}

Game::~Game()
{
	delete messages;
	delete currentArea;
	for (GameObject* object : gameObjects) {
		if (object != &player) {
			gameObjects.deleteObject(object);
		}
	}
}

void Game::drawFrame()
{
	SDL_RenderClear(renderer);

	if (!isGUIactive()) {
		redrawWorld();
	}

	redrawGUI();
	redrawMessages();

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderPresent(renderer);
}

void Game::drawWorld()
{
	SDL_SetRenderTarget(renderer, graphics.worldTexture);

	int width = currentArea->getWidth();
	int height = currentArea->getHeight();
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			Tile tile = currentArea->getTile(x, y);
			drawLetter(renderer, font, tile.letter, tile.color, x, y);
		}
	}

	SDL_SetRenderTarget(renderer, NULL);
}

void Game::drawObjects()
{
	SDL_SetRenderTarget(renderer, graphics.objectsTexture);
	SDL_RenderSetScale(renderer, options.general.scale, options.general.scale);
	SDL_RenderClear(renderer);

	Position playerPosition = player.getPosition();
	for (GameObject* object : gameObjects) {
		Position objectPosition = object->getPosition();
		int realX = objectPosition.x - playerPosition.x + options.gui.centerX;
		int realY = objectPosition.y - playerPosition.y + options.gui.centerY;
		drawLetter(renderer, font, object->getLetter(), object->getColor(), realX, realY);
	}

	int dirX = DIR_X(playerPosition.direction);
	int dirY = DIR_Y(playerPosition.direction);
	drawLetter(renderer, font, options.game.targetLetter, TARGET_COLOR, options.gui.centerX + dirX, options.gui.centerY + dirY);

	SDL_SetRenderTarget(renderer, NULL);
}

void Game::drawGUI()
{
	SDL_SetRenderTarget(renderer, graphics.guiTexture);
	SDL_RenderSetScale(renderer, options.general.scale, options.general.scale);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	Position playerPosition = player.getPosition();
	drawRectangle(renderer, GUI_RECTANGLE_COLOR, options.gui.guiXOffset, options.gui.guiYOffset, options.general.screenWidth - 2 * options.gui.guiXOffset, options.gui.tileHeight * 3);
	drawRectangle(renderer, GUI_RECTANGLE_COLOR, options.gui.guiXOffset, options.general.screenHeight - options.gui.guiYOffset - 3 * options.gui.tileHeight, options.general.screenWidth - 2 * options.gui.guiXOffset, options.gui.tileHeight * 3);
	drawText(renderer, font, text[ {TextCategory::Area, currentArea->getNameID()} ], COLOR_WHITE, 2 * options.gui.guiXOffset, options.gui.guiYOffset + options.gui.tileHeight / 2, Alignment::Left);

	std::stringstream nextLevelText;
	nextLevelText << text[String::Next] << player.creature.getXPRemaining();
	drawText(renderer, font, nextLevelText.str(), COLOR_YELLOW, options.general.screenWidth / 2, options.gui.guiYOffset + options.gui.tileHeight / 2, Alignment::Center);

	std::stringstream hpText;
	hpText << text[String::HP] << player.creature.getHPCurrent() << "/" << player.creature.getHPMax();
	drawText(renderer, font, hpText.str(), COLOR_RED, options.general.screenWidth - 2 * options.gui.guiXOffset, options.gui.guiYOffset + options.gui.tileHeight / 2, Alignment::Right);

	std::stringstream mpText;
	mpText << text[String::MP] << player.creature.getMPCurrent() << "/" << player.creature.getMPMax();
	drawText(renderer, font, mpText.str(), COLOR_BLUE, options.general.screenWidth - 2 * options.gui.guiXOffset, options.gui.guiYOffset + 3 * options.gui.tileHeight / 2, Alignment::Right);

	std::stringstream locationText;
	locationText << text[String::X] << playerPosition.x << text[String::Y] << playerPosition.y;
	drawText(renderer, font, locationText.str(), COLOR_YELLOW, options.general.screenWidth / 2, options.gui.guiYOffset + 3 * options.gui.tileHeight / 2, Alignment::Center);

	std::stringstream playerInfoText;
	playerInfoText << player.getName() << text[String::level] << player.creature.getLevel();
	drawText(renderer, font, playerInfoText.str(), COLOR_WHITE, 2 * options.gui.guiXOffset, options.general.screenHeight - options.gui.guiYOffset - 5 * options.gui.tileHeight / 2, Alignment::Left);

	std::stringstream weaponInUseText;
	weaponInUseText << text[String::WeaponInUse] << text[ {TextCategory::Item, player.creature.getWeaponNameID()} ];
	drawText(renderer, font, weaponInUseText.str(), COLOR_DGREEN, 2 * options.gui.guiXOffset, options.general.screenHeight - options.gui.guiYOffset - 3 * options.gui.tileHeight / 2, Alignment::Left);

	int dirX = DIR_X(playerPosition.direction);
	int dirY = DIR_Y(playerPosition.direction);
	if (!currentArea->isTileOutside(playerPosition.x + dirX, playerPosition.y + dirY)) {
		GameObjects objects = player.isPositionTaken(playerPosition.x + dirX, playerPosition.y + dirY);
		if (!objects.empty()) {
			GameObject* object = objects[0];
			TextPair textPair = object->getText();
			drawText(renderer, font, text[textPair], object->getColor(), 2 * options.gui.guiXOffset, options.general.screenHeight - options.gui.guiYOffset - options.gui.tileHeight / 2, Alignment::Left);
		} else {
			Tile objectTile = currentArea->getTile(playerPosition.x + dirX, playerPosition.y + dirY);
			drawText(renderer, font, text[ {TextCategory::Object, objectTile.nameID} ], objectTile.color, 2 * options.gui.guiXOffset, options.general.screenHeight - options.gui.guiYOffset - options.gui.tileHeight / 2, Alignment::Left);
		}
	}

	std::stringstream shortcutsText;
	shortcutsText << text[String::SHORTCUTS];
	drawText(renderer, font, shortcutsText.str(), COLOR_BROWN, options.general.screenWidth - 2 * options.gui.guiXOffset, options.general.screenHeight - options.gui.guiYOffset - 5 * options.gui.tileHeight / 2, Alignment::Right);

	if (player.creature.getAbilityPoints() > 0) {
		std::stringstream newLevelText;
		newLevelText << text[String::NextLevel];
		drawText(renderer, font, newLevelText.str(), COLOR_LGREEN, options.general.screenWidth - 2 * options.gui.guiXOffset, options.general.screenHeight - options.gui.guiYOffset - 3 * options.gui.tileHeight / 2, Alignment::Right);
	}

	std::stringstream fpsText;
	fpsText << text[String::FPS] << std::setprecision(2) << std::fixed << timer.getFPS();
	drawText(renderer, font, fpsText.str(), COLOR_GRAY, options.general.screenWidth - 2 * options.gui.guiXOffset, options.general.screenHeight - options.gui.guiYOffset - options.gui.tileHeight / 2, Alignment::Right);


	if (isGUIactive()) {
		drawRectangle(renderer, COLOR_WHITE, options.gui.tabXOffset - 4 * options.general.scale, options.gui.tabYOffset - 4 * options.general.scale, options.gui.tabWidth + 8 * options.general.scale, options.gui.tabHeight + 8 * options.general.scale, true);
		drawRectangle(renderer, GUI_RECTANGLE_COLOR, options.gui.tabXOffset, options.gui.tabYOffset, options.gui.tabWidth, options.gui.tabHeight);
	}

	switch (activeTab) {
	case GUI::Inventory: {
		drawInventory();
		break;
	}
	case GUI::Character: {
		drawCharacterInfo();
		break;
	}
	case GUI::Map:
		drawMap();
		break;
	default:
		break;
	}

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_SetRenderTarget(renderer, NULL);
}

void Game::drawCharacterInfo()
{
	int line = 0;
	const int xOffset = options.gui.tabXOffset + 2 * options.general.scale;
	const int yOffset = options.gui.tabYOffset + 0.5f * options.gui.tileHeight;

	std::stringstream playerInfoText;
	playerInfoText << player.getName() << text[String::level] << player.creature.getLevel() << ". " << text[genderNameIDs[(unsigned int)(player.creature.getGender())]];
	drawText(renderer, font, playerInfoText.str(), COLOR_WHITE, xOffset, yOffset + (options.gui.tileHeight * line++));

	std::stringstream creatureInfoText;
	creatureInfoText << text[String::Alignment] << text[alignmentNameIDs[(unsigned int)(player.creature.getAlignment())]];
	drawText(renderer, font, creatureInfoText.str(), COLOR_WHITE, xOffset, yOffset + (options.gui.tileHeight * line++));

	std::stringstream xpText;
	xpText << text[String::Experience] << player.creature.getXPCurrent() << "/" << player.creature.getXPCurrent() + player.creature.getXPRemaining();
	drawText(renderer, font, xpText.str(), COLOR_YELLOW, xOffset, yOffset + (options.gui.tileHeight * line++));

	std::stringstream hpText;
	hpText << text[String::HealthPoints] << player.creature.getHPCurrent() << "/" << player.creature.getHPMax();
	drawText(renderer, font, hpText.str(), COLOR_RED, xOffset, yOffset + (options.gui.tileHeight * line++));

	std::stringstream hpRegenerationText;
	hpRegenerationText << text[String::HealthRegeneration] << player.creature.getHPRegeneration();
	drawText(renderer, font, hpRegenerationText.str(), COLOR_RED, xOffset, yOffset + (options.gui.tileHeight * line++));

	std::stringstream mpText;
	mpText << text[String::ManaPoints] << player.creature.getHPCurrent() << "/" << player.creature.getHPMax();
	drawText(renderer, font, mpText.str(), COLOR_BLUE, xOffset, yOffset + (options.gui.tileHeight * line++));

	std::stringstream mpRegenerationText;
	mpRegenerationText << text[String::ManaRegeneration] << player.creature.getMPRegeneration();
	drawText(renderer, font, mpRegenerationText.str(), COLOR_BLUE, xOffset, yOffset + (options.gui.tileHeight * line++));

	std::stringstream damageText;
	damageText << text[String::Damage] << player.creature.getDamageMin() << "-" << player.creature.getDamageMax();
	drawText(renderer, font, damageText.str(), COLOR_BROWN, xOffset, yOffset + (options.gui.tileHeight * line++));

	std::stringstream attackRateText;
	attackRateText << text[String::AttackRate] << player.creature.getAttackRate();
	drawText(renderer, font, attackRateText.str(), COLOR_BROWN, xOffset, yOffset + (options.gui.tileHeight * line++));

	std::stringstream defenseText;
	defenseText << text[String::Defense] << player.creature.getDefense();
	drawText(renderer, font, defenseText.str(), COLOR_BROWN, xOffset, yOffset + (options.gui.tileHeight * line++));

	std::stringstream defenseRateText;
	defenseRateText << text[String::DefenseRate] << player.creature.getDefenseRate();
	drawText(renderer, font, defenseRateText.str(), COLOR_BROWN, xOffset, yOffset + (options.gui.tileHeight * line++));

	std::stringstream goldText;
	goldText << text[String::Gold] << player.creature.getGold();
	drawText(renderer, font, goldText.str(), COLOR_YELLOW, xOffset, yOffset + (options.gui.tileHeight * line++));

	if (player.creature.getAbilityPoints() > 0) {
		drawRectangle(renderer, COLOR_DGREEN, options.gui.tabXOffset, options.gui.tabYOffset + options.gui.tabHeight + (-5.0f + characterInfoPosition) * options.gui.tileHeight, options.gui.tabWidth, options.gui.tileHeight);
	}

	for (size_t abilityIndex = 0; abilityIndex < Ability::count; ++abilityIndex) {
		Ability ability = Ability(abilityIndex);
		drawText(renderer, font, text[ {TextCategory::General, size_t(String::Strength) + abilityIndex} ], COLOR_YGREEN, xOffset, options.gui.tabYOffset + options.gui.tabHeight + (-4.5f + ability) * options.gui.tileHeight);
		drawText(renderer, font, STRING(player.creature.getAbilityValue(ability)), COLOR_YGREEN, options.gui.tabXOffset + options.gui.tabWidth - 2 * options.general.scale, options.gui.tabYOffset + options.gui.tabHeight + (-4.5f + ability) * options.gui.tileHeight, Alignment::Right);
	}
}

void Game::drawInventory()
{
	Inventory& inventory = player.creature.inventory;
	if (!inventory.isEmpty()) {
		int inventoryPage = inventoryPosition / options.inventory.itemsPerPage;
		drawRectangle(renderer, GUI_INVENTORY_COLOR, options.gui.tabXOffset, options.gui.tabYOffset + (inventoryPosition % options.inventory.itemsPerPage) * options.gui.tileHeight, options.gui.tabWidth / 2, options.gui.tileHeight);
		for (unsigned int index = 0; int(index) < options.inventory.itemsPerPage; ++index) {
			if (index + inventoryPage * options.inventory.itemsPerPage >= inventory.getBackpackSize()) {
				break;
			}

			Item* item = inventory.getBackpackItem(index + inventoryPage * options.inventory.itemsPerPage);
			SDL_Color sdlColor = (player.creature.isItemEquipped(item) ? SDL_Color(COLOR_BLUE) : item->getColor());
			drawText(renderer, font, text[ {TextCategory::Item, item->getNameID()} ], sdlColor, options.gui.tabXOffset + 2 * options.general.scale, options.gui.tabYOffset + (0.5f + index) * options.gui.tileHeight, Alignment::Left, Alignment::Center);
		}

		Item* selectedItem = inventory.getBackpackItem(inventoryPosition);
		drawItemDescription(selectedItem);
	} else {
		drawText(renderer, font, text[String::EmptyBackpack], COLOR_BROWN, options.gui.tabXOffset + 2 * options.general.scale, options.gui.tabYOffset + 0.5f * options.gui.tileHeight, Alignment::Left, Alignment::Center);
	}

	std::stringstream goldText;
	goldText << text[String::Gold] << player.creature.getGold();
	drawText(renderer, font, goldText.str(), COLOR_YELLOW, options.gui.tabXOffset + options.gui.tabWidth - 2 * options.general.scale, options.gui.tabYOffset + options.gui.tabHeight - 0.5f * options.gui.tileHeight, Alignment::Right);
}

void Game::drawItemDescription(Item *item)
{
	int line = 0;
	const int xOffset = options.gui.tabXOffset + 3 * options.gui.tabWidth / 4;
	const int yOffset = options.gui.tabYOffset + 0.5f * options.gui.tileHeight;
	drawText(renderer, font, text[ {TextCategory::Item, item->getNameID()} ], item->getColor(), xOffset, yOffset + (options.gui.tileHeight * line++), Alignment::Center);

	std::stringstream descriptionText;
	if (item->getDescriptionID() > 0) {
		descriptionText << text[ {TextCategory::Item, item->getDescriptionID()} ];
	} else {
		switch (item->getType()) {
		case ItemType::weapon:
		case ItemType::armor:
		case ItemType::quick:
			descriptionText << text[categoryNameIDs[(unsigned int)(item->getCategory())]];
			break;
		default:
			descriptionText << text[typeNameIDs[(unsigned int)(item->getType())]];
			break;
		}
	}

	drawText(renderer, font, descriptionText.str(), item->getColor(), xOffset, yOffset + (options.gui.tileHeight * line++), Alignment::Center);

	if (item->getRequiredLevel() > 0) {
		std::stringstream requiredLevelText;
		requiredLevelText << text[String::RequiredLevel] << item->getRequiredLevel();
		SDL_Color sdlColor = (item->getRequiredLevel() > player.creature.getLevel() ? SDL_Color(COLOR_RED) : SDL_Color(COLOR_WHITE));
		drawText(renderer, font, requiredLevelText.str(), sdlColor, xOffset, yOffset + (options.gui.tileHeight * line++), Alignment::Center);
	}

	if (item->getDamage() + item->getDamageDelta() > 0) {
		std::stringstream attackText, attackRateText;
		attackText << text[String::Damage] << item->getDamage() << "-" << item->getDamage() + item->getDamageDelta();
		attackRateText << text[String::AttackRate] << item->getAttackRate();
		drawText(renderer, font, attackText.str(), COLOR_BROWN, xOffset, yOffset + (options.gui.tileHeight * line++), Alignment::Center);
		drawText(renderer, font, attackRateText.str(), COLOR_BROWN, xOffset, yOffset + (options.gui.tileHeight * line++), Alignment::Center);
	}

	if (item->getDefense() + item->getDefenseRate() > 0) {
		std::stringstream defenseText, defenseRateText;
		defenseText << text[String::Defense] << item->getDefense();
		defenseRateText << text[String::DefenseRate] << item->getDefenseRate();
		drawText(renderer, font, defenseText.str(), COLOR_BROWN, xOffset, yOffset + (options.gui.tileHeight * line++), Alignment::Center);
		drawText(renderer, font, defenseRateText.str(), COLOR_BROWN, xOffset, yOffset + (options.gui.tileHeight * line++), Alignment::Center);
	}

	if (item->getDelay() > 0) {
		std::stringstream delayText;
		delayText << text[item->getType() == ItemType::weapon ? String::Delay : String::Speed] << item->getDelay();
		drawText(renderer, font, delayText.str(), COLOR_BROWN, xOffset, yOffset + (options.gui.tileHeight * line++), Alignment::Center);
	}

	drawText(renderer, font, text[String::Def], COLOR_BROWN, options.gui.tabXOffset + 3 * options.gui.tabWidth / 4 - 2 * options.inventory.abilitiesDistance * options.gui.tileWidth, options.gui.tabYOffset + (-1.5f + options.inventory.itemsPerPage) * options.gui.tileHeight, Alignment::Center);
	drawText(renderer, font, text[String::DefRate], COLOR_BROWN, options.gui.tabXOffset + 3 * options.gui.tabWidth / 4 - options.inventory.abilitiesDistance * options.gui.tileWidth, options.gui.tabYOffset + (-1.5f + options.inventory.itemsPerPage) * options.gui.tileHeight, Alignment::Center);
	drawText(renderer, font, text[String::AttMin], COLOR_BROWN, options.gui.tabXOffset + 3 * options.gui.tabWidth / 4, options.gui.tabYOffset + (-1.5f + options.inventory.itemsPerPage) * options.gui.tileHeight, Alignment::Center);
	drawText(renderer, font, text[String::AttMax], COLOR_BROWN, options.gui.tabXOffset + 3 * options.gui.tabWidth / 4 + options.inventory.abilitiesDistance * options.gui.tileWidth, options.gui.tabYOffset + (-1.5f + options.inventory.itemsPerPage) * options.gui.tileHeight, Alignment::Center);
	drawText(renderer, font, text[String::Acc], COLOR_BROWN, options.gui.tabXOffset + 3 * options.gui.tabWidth / 4 + 2 * options.inventory.abilitiesDistance * options.gui.tileWidth, options.gui.tabYOffset + (-1.5f + options.inventory.itemsPerPage) * options.gui.tileHeight, Alignment::Center);

	drawText(renderer, font, STRING(player.creature.getDefense()), COLOR_BROWN, options.gui.tabXOffset + 3 * options.gui.tabWidth / 4 - 2 * options.inventory.abilitiesDistance * options.gui.tileWidth, options.gui.tabYOffset + (-0.5f + options.inventory.itemsPerPage) * options.gui.tileHeight, Alignment::Center);
	drawText(renderer, font, STRING(player.creature.getDefenseRate()), COLOR_BROWN, options.gui.tabXOffset + 3 * options.gui.tabWidth / 4 - options.inventory.abilitiesDistance * options.gui.tileWidth, options.gui.tabYOffset + (-0.5f + options.inventory.itemsPerPage) * options.gui.tileHeight, Alignment::Center);
	drawText(renderer, font, STRING(player.creature.getDamageMin()), COLOR_BROWN, options.gui.tabXOffset + 3 * options.gui.tabWidth / 4, options.gui.tabYOffset + (-0.5f + options.inventory.itemsPerPage) * options.gui.tileHeight, Alignment::Center);
	drawText(renderer, font, STRING(player.creature.getDamageMax()), COLOR_BROWN, options.gui.tabXOffset + 3 * options.gui.tabWidth / 4 + options.inventory.abilitiesDistance * options.gui.tileWidth, options.gui.tabYOffset + (-0.5f + options.inventory.itemsPerPage) * options.gui.tileHeight, Alignment::Center);
	drawText(renderer, font, STRING(player.creature.getAttackRate()), COLOR_BROWN, options.gui.tabXOffset + 3 * options.gui.tabWidth / 4 + 2 * options.inventory.abilitiesDistance * options.gui.tileWidth, options.gui.tabYOffset + (-0.5f + options.inventory.itemsPerPage) * options.gui.tileHeight, Alignment::Center);

	for (size_t abilityIndex = 0; abilityIndex < Ability::count; ++abilityIndex) {
		Ability ability = Ability(abilityIndex);
		drawText(renderer, font, text[ {TextCategory::General, size_t(String::STR) + abilityIndex} ], COLOR_WHITE, options.gui.tabXOffset + options.gui.tabWidth / 2 + (abilityIndex - float(Ability::count - 1) / 2) * options.inventory.abilitiesDistance * options.gui.tileWidth, options.gui.tabYOffset + (0.5f + options.inventory.itemsPerPage) * options.gui.tileHeight, Alignment::Center);
		drawText(renderer, font, STRING(player.creature.getAbilityValue(ability)), COLOR_WHITE, options.gui.tabXOffset + options.gui.tabWidth / 2 + (ability - float(Ability::count - 1) / 2) * options.inventory.abilitiesDistance * options.gui.tileWidth, options.gui.tabYOffset + (1.5f + options.inventory.itemsPerPage) * options.gui.tileHeight, Alignment::Center);

		SDL_Color sdlColor = (item->getRequiredAbility(ability) > player.creature.getAbilityValue(ability) ? SDL_Color(COLOR_RED) : SDL_Color(COLOR_GREEN));
		drawText(renderer, font, STRING(item->getRequiredAbility(ability)), sdlColor, options.gui.tabXOffset + options.gui.tabWidth / 2 + (ability - float(Ability::count - 1) / 2) * options.inventory.abilitiesDistance * options.gui.tileWidth, options.gui.tabYOffset + (2.5f + options.inventory.itemsPerPage) * options.gui.tileHeight, Alignment::Center);
	}
}

void Game::drawMap()
{
	size_t width = currentArea->getWidth();
	size_t height = currentArea->getHeight();
	Position playerPosition = player.getPosition() + mapOffset;
	int xStart = std::max(0, -options.gui.tabWidth / (2 * options.map.pixelSize) + playerPosition.x);
	int yStart = std::max(0, -options.gui.tabHeight / (2 * options.map.pixelSize) + playerPosition.y);
	int xEnd = std::min(int(width), options.gui.tabWidth / (2 * options.map.pixelSize) + playerPosition.x);
	int yEnd = std::min(int(height), options.gui.tabHeight / (2 * options.map.pixelSize) + playerPosition.y);
	for (int y = yStart; y < yEnd; ++y) {
		for (int x = xStart; x < xEnd; ++x) {
			Tile tile = currentArea->getTile(x, y);
			SDL_Color sdlColor = tile.color;
			sdlColor.a = options.map.alpha;

			if (tile.letter != '\0' and tile.letter != ' ') {
				drawPixel(renderer, sdlColor, options.general.screenWidth / 2 + options.map.pixelSize * (x - playerPosition.x), options.general.screenHeight / 2 + options.map.pixelSize * (y - playerPosition.y), options.map.pixelSize);
			}
		}
	}

	Color playerColor = player.creature.getColor();
	int xPlayer = options.general.screenWidth / 2 - options.map.pixelSize * mapOffset.x;
	int yPlayer = options.general.screenHeight / 2 - options.map.pixelSize * mapOffset.y;
	if (xPlayer >= options.general.screenWidth / 2 - options.gui.tabWidth / 2 and xPlayer < options.general.screenWidth / 2 + options.gui.tabWidth / 2 and yPlayer >= options.general.screenHeight / 2 - options.gui.tabHeight / 2 and yPlayer < options.general.screenHeight / 2 + options.gui.tabHeight / 2) {
		drawPixel(renderer, playerColor, options.general.screenWidth / 2 - options.map.pixelSize * mapOffset.x, options.general.screenHeight / 2 - options.map.pixelSize * mapOffset.y, options.map.pixelSize);
	}
}

void Game::redrawWorld()
{
	Position playerPosition = player.getPosition();
	graphics.worldRectangle.x = options.gui.tileBaseWidth * (options.gui.centerX - playerPosition.x);
	graphics.worldRectangle.y = options.gui.tileBaseHeight * (options.gui.centerY - playerPosition.y);
	SDL_RenderCopy(renderer, graphics.worldTexture, &graphics.screenRectangle, &graphics.worldRectangle);
	SDL_RenderCopy(renderer, graphics.objectsTexture, NULL, NULL);
}

void Game::redrawGUI()
{
	SDL_RenderCopy(renderer, graphics.guiTexture, NULL, NULL);
}

void Game::redrawMessages()
{
	SDL_RenderCopy(renderer, graphics.messagesTexture, NULL, NULL);
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

	// draw world in advance
	drawWorld();
	drawObjects();
	drawGUI();
	while (!quit) {
		timer.update();
		if (timer.frame()) {
			while (SDL_PollEvent(&event) != 0) {
				update = true;
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
				mapOffset = {0, 0, 0};
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

					if (keyboard.isKeyPressed(SDLK_LCTRL)) {
						GameObjects objects = player.isPositionTaken(player.getPosition() + 1);
						if (!objects.empty()) {
							GameObject* object = objects[0];
							switch (object->type) {
							case ObjectType::Item: {
								ItemObject* itemObject = (ItemObject*)object;
								if (!player.creature.inventory.isFull()) {
									Item item = itemObject->item;
									player.creature.inventory.addItem(item);
									messages->add(text[String::AddedItem] + text[ {TextCategory::Item, item.getNameID()} ], COLOR_WHITE);
									gameObjects.deleteObject(object);
								} else {
									messages->add(text[String::BackpackIsFull], COLOR_LRED);
								}
								break;
							}
							case ObjectType::Door: {
								Door* door = (Door*)object;
								if (door->toggle()) {
									messages->add(text[String::Closed], door->getColor());
								}
							}
							default:
								break;
							}
						}
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
						inventoryPosition = std::max(0, inventoryPosition - options.inventory.itemsPerPage);
					}
					if (keyboard.isKey(SDLK_RIGHT) or keyboard.isKey(SDLK_KP_6) or keyboard.isKey(SDLK_PAGEDOWN)) {
						inventoryPosition = std::min((int)(player.creature.inventory.getBackpackSize()) - 1, inventoryPosition + options.inventory.itemsPerPage);
					}
					if (keyboard.isKeyPressed(SDLK_RETURN) or keyboard.isKeyPressed(SDLK_KP_ENTER)) {
						if (!player.creature.equipItem(inventoryPosition)) {
							messages->add(text[String::TooHighItemRequirements], COLOR_ORANGE);
						}
					}
				}

				break;
			case GUI::Character:
				if (player.creature.getAbilityPoints() > 0) {
					if (keyboard.isKey(SDLK_UP) or keyboard.isKey(SDLK_KP_8)) {
						characterInfoPosition = std::max(0, characterInfoPosition - 1);
					}
					if (keyboard.isKey(SDLK_DOWN) or keyboard.isKey(SDLK_KP_2)) {
						characterInfoPosition = std::min((int)(Ability::count) - 1, characterInfoPosition + 1);
					}
					if (keyboard.isKey(SDLK_LEFT) or keyboard.isKey(SDLK_KP_4) or keyboard.isKey(SDLK_PAGEUP)) {
						characterInfoPosition = 0;
					}
					if (keyboard.isKey(SDLK_RIGHT) or keyboard.isKey(SDLK_KP_6) or keyboard.isKey(SDLK_PAGEDOWN)) {
						characterInfoPosition = Ability::count - 1;
					}
					if (keyboard.isKeyPressed(SDLK_RETURN) or keyboard.isKeyPressed(SDLK_KP_ENTER)) {
						player.creature.assignPoint(Ability(characterInfoPosition));
					}
				}

				break;
			case GUI::Map:
				for (unsigned int dir = 0; dir < COUNT; ++dir) {
					Direction direction = Direction(dir);
					if (keyboard.isKey(keyDirection[direction])) {
						mapOffset.direction = direction;
						mapOffset = mapOffset + 1;
						break;
					}
				}

				break;
			}

			keyboard.step();

			for (GameObject* object : gameObjects) {
				object->step();
			}

			if (update) {
				drawObjects();
				drawGUI();
			}

			drawFrame();
			messages->step();
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

	int screenWidth = options.general.screenWidth;
	int screenHeight = options.general.screenHeight;
	if (options.general.fullscreen) {
		SDL_GetRendererOutputSize(renderer, &screenWidth, &screenHeight);
	}

	window = SDL_CreateWindow("Roguelike", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, 0);

	if (options.general.fullscreen) {
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_RenderSetScale(renderer, options.general.scale, options.general.scale);
	SDL_RenderPresent(renderer);

	if (renderer == NULL) {
		_LogError("Bad SDL object!");
		throw std::runtime_error("bad SDL object");
	}

	if (options.inventory.itemsPerPage <= 0) {
		_LogError("Too small inventory size!");
		throw std::runtime_error("too small inventory size");
	}

	graphics.screenRectangle.w = screenWidth;
	graphics.screenRectangle.h = screenHeight;
	graphics.worldRectangle.w = screenWidth;
	graphics.worldRectangle.h = screenHeight;

	int width = 0;
	int height = 0;
	if (currentArea != nullptr) {
		width = currentArea->getWidth();
		height = currentArea->getHeight();
	}  else {
		_LogError("An uninitialized area during graphic initialization!");
	}

	unsigned int realWidth = std::max(graphics.screenRectangle.w, width * options.gui.tileWidth);
	unsigned int realHeight = std::max(graphics.screenRectangle.h, height * options.gui.tileHeight);
	graphics.worldTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, realWidth, realHeight);
	graphics.messagesTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, screenWidth, screenHeight);
	graphics.objectsTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, screenWidth, screenHeight);
	graphics.guiTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, screenWidth, screenHeight);
	SDL_SetTextureBlendMode(graphics.messagesTexture, SDL_BLENDMODE_ADD);
	SDL_SetTextureBlendMode(graphics.objectsTexture, SDL_BLENDMODE_ADD);
	SDL_SetTextureBlendMode(graphics.worldTexture, SDL_BLENDMODE_ADD);
	SDL_SetTextureBlendMode(graphics.guiTexture, SDL_BLENDMODE_ADD);
}

void Game::quit()
{
	TTF_Quit();
	SDL_DestroyTexture(graphics.guiTexture);
	SDL_DestroyTexture(graphics.messagesTexture);
	SDL_DestroyTexture(graphics.objectsTexture);
	SDL_DestroyTexture(graphics.worldTexture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
