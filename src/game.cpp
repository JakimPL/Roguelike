#include "game.hpp"
#include "functions.hpp"
#include "graphics.hpp"
#include "log.hpp"
#include "message.hpp"
#include "options.hpp"

#include "objects/door.hpp"
#include "objects/itemobject.hpp"

#include <iomanip>

using namespace Graphics;
using namespace Functions;

Game::Game() : player(gameObjects, Creature(), STARTING_POSITION, "Liop")
{
	currentArea = new Area(gameObjects, "MOONDALE");

	initializeGraphics();
	initializeFont();

	messages = new Messages(renderer, graphics.messagesTexture, font);
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

int Game::clampX(int x)
{
	return std::min(0, std::max(options.general.screenWidth / (2 * options.gui.tileBaseWidth) - int(currentArea->getWidth()), x));
}

int Game::clampY(int y)
{
	return std::min(3, std::max(options.general.screenHeight / (2 * options.gui.tileBaseHeight) - int(currentArea->getHeight() + 3), y));
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
	int x = clampX(options.gui.centerX - playerPosition.x);
	int y = clampY(options.gui.centerY - playerPosition.y);

	for (GameObject* object : gameObjects) {
		Position objectPosition = object->getPosition();
		int realX = objectPosition.x + x;
		int realY = objectPosition.y + y;
		drawLetter(renderer, font, object->getLetter(), object->getColor(), realX, realY);
	}

	int dirX = DIR_X(playerPosition.direction);
	int dirY = DIR_Y(playerPosition.direction);
	drawLetter(renderer, font, options.game.targetLetter, TARGET_COLOR, playerPosition.x + x + dirX, playerPosition.y + y + dirY);

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
	nextLevelText << text[s_Next] << player.creature.getXPRemaining();
	drawText(renderer, font, nextLevelText.str(), COLOR_YELLOW, options.general.screenWidth / 2, options.gui.guiYOffset + options.gui.tileHeight / 2, Alignment::Center);

	std::stringstream hpText;
	hpText << text[s_HP] << player.creature.getHPCurrent() << "/" << player.creature.getHPMax();
	drawText(renderer, font, hpText.str(), COLOR_RED, options.general.screenWidth - 2 * options.gui.guiXOffset, options.gui.guiYOffset + options.gui.tileHeight / 2, Alignment::Right);

	std::stringstream mpText;
	mpText << text[s_MP] << player.creature.getMPCurrent() << "/" << player.creature.getMPMax();
	drawText(renderer, font, mpText.str(), COLOR_BLUE, options.general.screenWidth - 2 * options.gui.guiXOffset, options.gui.guiYOffset + 3 * options.gui.tileHeight / 2, Alignment::Right);

	std::stringstream locationText;
	locationText << text[s_X] << playerPosition.x << text[s_Y] << playerPosition.y;
	drawText(renderer, font, locationText.str(), COLOR_YELLOW, options.general.screenWidth / 2, options.gui.guiYOffset + 3 * options.gui.tileHeight / 2, Alignment::Center);

	std::stringstream playerInfoText;
	playerInfoText << player.getName() << text[s_level] << player.creature.getLevel();
	drawText(renderer, font, playerInfoText.str(), COLOR_WHITE, 2 * options.gui.guiXOffset, options.general.screenHeight - options.gui.guiYOffset - 5 * options.gui.tileHeight / 2, Alignment::Left);

	std::stringstream weaponInUseText;
	weaponInUseText << text[s_WeaponInUse] << text[ {TextCategory::Item, player.creature.getWeaponNameID()} ];
	drawText(renderer, font, weaponInUseText.str(), COLOR_DGREEN, 2 * options.gui.guiXOffset, options.general.screenHeight - options.gui.guiYOffset - 3 * options.gui.tileHeight / 2, Alignment::Left);

	int dirX = DIR_X(playerPosition.direction);
	int dirY = DIR_Y(playerPosition.direction);
	if (!currentArea->isTileOutside(playerPosition.x + dirX, playerPosition.y + dirY)) {
		GameObjects objects = currentArea->isPositionTaken(playerPosition.x + dirX, playerPosition.y + dirY);
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
	shortcutsText << text[s_SHORTCUTS];
	drawText(renderer, font, shortcutsText.str(), COLOR_BROWN, options.general.screenWidth - 2 * options.gui.guiXOffset, options.general.screenHeight - options.gui.guiYOffset - 5 * options.gui.tileHeight / 2, Alignment::Right);

	if (player.creature.getAbilityPoints() > 0) {
		std::stringstream newLevelText;
		newLevelText << text[s_NextLevel];
		drawText(renderer, font, newLevelText.str(), COLOR_LGREEN, options.general.screenWidth - 2 * options.gui.guiXOffset, options.general.screenHeight - options.gui.guiYOffset - 3 * options.gui.tileHeight / 2, Alignment::Right);
	}

	std::stringstream fpsText;
	fpsText << text[s_FPS] << std::setprecision(2) << std::fixed << timer.getFPS();
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
	case GUI::Dialog: {
		drawDialog();
		break;
	}
	case GUI::Map: {
		drawMap();
		break;
	}
	case GUI::Store: {
		drawStore();
		break;
	}
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
	playerInfoText << player.getName() << text[s_level] << player.creature.getLevel() << ". " << text[genderNameIDs[(unsigned int)(player.creature.getGender())]];
	drawText(renderer, font, playerInfoText.str(), COLOR_WHITE, xOffset, yOffset + (options.gui.tileHeight * line++));

	std::stringstream creatureInfoText;
	creatureInfoText << text[s_Alignment] << text[alignmentNameIDs[(unsigned int)(player.creature.getAlignment())]];
	drawText(renderer, font, creatureInfoText.str(), COLOR_WHITE, xOffset, yOffset + (options.gui.tileHeight * line++));

	std::stringstream xpText;
	xpText << text[s_Experience] << player.creature.getXPCurrent() << "/" << player.creature.getXPCurrent() + player.creature.getXPRemaining();
	drawText(renderer, font, xpText.str(), COLOR_YELLOW, xOffset, yOffset + (options.gui.tileHeight * line++));

	std::stringstream hpText;
	hpText << text[s_HealthPoints] << player.creature.getHPCurrent() << "/" << player.creature.getHPMax();
	drawText(renderer, font, hpText.str(), COLOR_RED, xOffset, yOffset + (options.gui.tileHeight * line++));

	std::stringstream hpRegenerationText;
	hpRegenerationText << text[s_HealthRegeneration] << player.creature.getHPRegeneration();
	drawText(renderer, font, hpRegenerationText.str(), COLOR_RED, xOffset, yOffset + (options.gui.tileHeight * line++));

	std::stringstream mpText;
	mpText << text[s_ManaPoints] << player.creature.getHPCurrent() << "/" << player.creature.getHPMax();
	drawText(renderer, font, mpText.str(), COLOR_BLUE, xOffset, yOffset + (options.gui.tileHeight * line++));

	std::stringstream mpRegenerationText;
	mpRegenerationText << text[s_ManaRegeneration] << player.creature.getMPRegeneration();
	drawText(renderer, font, mpRegenerationText.str(), COLOR_BLUE, xOffset, yOffset + (options.gui.tileHeight * line++));

	std::stringstream damageText;
	damageText << text[s_Damage] << player.creature.getDamageMin() << "-" << player.creature.getDamageMax();
	drawText(renderer, font, damageText.str(), COLOR_BROWN, xOffset, yOffset + (options.gui.tileHeight * line++));

	std::stringstream attackRateText;
	attackRateText << text[s_AttackRate] << player.creature.getAttackRate();
	drawText(renderer, font, attackRateText.str(), COLOR_BROWN, xOffset, yOffset + (options.gui.tileHeight * line++));

	std::stringstream defenseText;
	defenseText << text[s_Defense] << player.creature.getDefense();
	drawText(renderer, font, defenseText.str(), COLOR_BROWN, xOffset, yOffset + (options.gui.tileHeight * line++));

	std::stringstream defenseRateText;
	defenseRateText << text[s_DefenseRate] << player.creature.getDefenseRate();
	drawText(renderer, font, defenseRateText.str(), COLOR_BROWN, xOffset, yOffset + (options.gui.tileHeight * line++));

	std::stringstream goldText;
	goldText << text[s_Gold] << player.creature.getGold();
	drawText(renderer, font, goldText.str(), COLOR_YELLOW, xOffset, yOffset + (options.gui.tileHeight * line++));

	if (player.creature.getAbilityPoints() > 0) {
		drawRectangle(renderer, COLOR_DGREEN, options.gui.tabXOffset, options.gui.tabYOffset + options.gui.tabHeight + (-5.0f + characterInfoPosition) * options.gui.tileHeight, options.gui.tabWidth, options.gui.tileHeight);
	}

	for (size_t abilityIndex = 0; abilityIndex < Ability::count; ++abilityIndex) {
		Ability ability = Ability(abilityIndex);
		drawText(renderer, font, text[ {TextCategory::General, size_t(s_Strength) + abilityIndex} ], COLOR_YGREEN, xOffset, options.gui.tabYOffset + options.gui.tabHeight + (-4.5f + ability) * options.gui.tileHeight);
		drawText(renderer, font, STRING(player.creature.getAbilityValue(ability)), COLOR_YGREEN, options.gui.tabXOffset + options.gui.tabWidth - 2 * options.general.scale, options.gui.tabYOffset + options.gui.tabHeight + (-4.5f + ability) * options.gui.tileHeight, Alignment::Right);
	}
}

void Game::drawDialog()
{
	int line = 0;
	const int xOffset = options.gui.tabXOffset + 2 * options.general.scale;
	const int yOffset = options.gui.tabYOffset + 0.5f * options.gui.tileHeight;

	std::stringstream npcNameText;
	npcNameText << text[ {TextCategory::Creature, targetObject->getNameID()} ] << ":";
	drawText(renderer, font, npcNameText.str(), targetObject->getColor(), xOffset, yOffset + (options.gui.tileHeight * line++));

	line++;

	drawText(renderer, font, text[ {TextCategory::Dialog, currentDialog->getLineTextID(dialogID)} ], DIALOG_COLOR, xOffset, yOffset + (options.gui.tileHeight * line++));
	DialogLine dialogLine = currentDialog->getLine(dialogID);
	unsigned int size = dialogLine.responses.size();
	drawRectangle(renderer, COLOR_DGRAY, options.gui.tabXOffset, options.gui.tabYOffset + options.gui.tabHeight + (responsePosition - size) * options.gui.tileHeight, options.gui.tabWidth, options.gui.tileHeight);

	for (unsigned int responseIndex = 0; responseIndex < size; ++responseIndex) {
		Response response = currentDialog->getLineResponse(dialogID, responseIndex);
		std::stringstream responseText;
		responseText << responseIndex + 1 << ". " << text[ {TextCategory::Dialog, response.textID }];
		drawText(renderer, font, responseText.str(), DIALOG_COLOR, xOffset, options.gui.tabYOffset + options.gui.tabHeight + (0.5f + responseIndex - size) * options.gui.tileHeight);
	}
}

void Game::drawInventory()
{
	Inventory& inventory = player.creature.inventory;
	bool activeStore = (activeTab == GUI::Store and storeTab);
	inventoryPage = inventoryPosition / options.inventory.itemsPerPage;
	if (activeStore) {
		storePage = storePosition / options.inventory.storeItemsPerPage;
	}

	int offset = options.gui.tabWidth / 2 * activeStore;
	int position = (activeStore ? storePosition : inventoryPosition);

	if (!inventory.isEmpty() or activeStore) {
		drawRectangle(renderer, GUI_INVENTORY_COLOR, options.gui.tabXOffset + offset, options.gui.tabYOffset + (position % (activeStore ? options.inventory.storeItemsPerPage : options.inventory.itemsPerPage)) * options.gui.tileHeight, options.gui.tabWidth / 2, options.gui.tileHeight);
	}

	for (unsigned int index = 0; int(index) < options.inventory.itemsPerPage; ++index) {
		if (index + inventoryPage * options.inventory.itemsPerPage >= inventory.getBackpackSize()) {
			break;
		}

		int realIndex = index + inventoryPage * options.inventory.itemsPerPage;
		Item* item = inventory.getBackpackItem(realIndex);
		SDL_Color sdlColor = (player.creature.isItemEquipped(realIndex) ? SDL_Color(COLOR_BLUE) : item->getColor());
		drawText(renderer, font, text[ {TextCategory::Item, item->getNameID()} ], sdlColor, options.gui.tabXOffset + 2 * options.general.scale, options.gui.tabYOffset + (0.5f + index) * options.gui.tileHeight, Alignment::Left, Alignment::Center);
	}

	if (activeTab == GUI::Store) {
		for (unsigned int index = 0; int(index) < options.inventory.storeItemsPerPage; ++index) {
			if (index + storePage * options.inventory.storeItemsPerPage >= currentStore->inventory.getBackpackSize()) {
				break;
			}

			Item* item = currentStore->inventory.getBackpackItem(index + storePage * options.inventory.storeItemsPerPage);
			drawText(renderer, font, text[ {TextCategory::Item, item->getNameID()} ], item->getColor(), options.gui.tabXOffset + options.gui.tabWidth - 2 * options.general.scale, options.gui.tabYOffset + (0.5f + index) * options.gui.tileHeight, Alignment::Right, Alignment::Center);
		}
	}

	if (!inventory.isEmpty() or activeStore) {
		Item* selectedItem = activeStore ? currentStore->inventory.getBackpackItem(storePosition) : inventory.getBackpackItem(inventoryPosition);
		drawItemDescription(selectedItem);
	} else {
		drawText(renderer, font, text[s_EmptyBackpack], COLOR_BROWN, options.gui.tabXOffset + 2 * options.general.scale, options.gui.tabYOffset + 0.5f * options.gui.tileHeight, Alignment::Left, Alignment::Center);
	}

	std::stringstream goldText;
	goldText << text[s_Gold] << player.creature.getGold();
	drawText(renderer, font, goldText.str(), COLOR_YELLOW, options.gui.tabXOffset + options.gui.tabWidth - 2 * options.general.scale, options.gui.tabYOffset + options.gui.tabHeight - 0.5f * options.gui.tileHeight, Alignment::Right);
}

void Game::drawItemDescription(Item *item)
{
	if (activeTab == GUI::Inventory) {
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
			requiredLevelText << text[s_RequiredLevel] << item->getRequiredLevel();
			SDL_Color sdlColor = (item->getRequiredLevel() > player.creature.getLevel() ? SDL_Color(COLOR_RED) : SDL_Color(COLOR_WHITE));
			drawText(renderer, font, requiredLevelText.str(), sdlColor, xOffset, yOffset + (options.gui.tileHeight * line++), Alignment::Center);
		}

		if (item->getDamage() + item->getDamageDelta() > 0) {
			std::stringstream attackText, attackRateText;
			attackText << text[s_Damage] << item->getDamage() << "-" << item->getDamage() + item->getDamageDelta();
			attackRateText << text[s_AttackRate] << item->getAttackRate();
			drawText(renderer, font, attackText.str(), COLOR_BROWN, xOffset, yOffset + (options.gui.tileHeight * line++), Alignment::Center);
			drawText(renderer, font, attackRateText.str(), COLOR_BROWN, xOffset, yOffset + (options.gui.tileHeight * line++), Alignment::Center);
		}

		if (item->getDefense() + item->getDefenseRate() > 0) {
			std::stringstream defenseText, defenseRateText;
			defenseText << text[s_Defense] << item->getDefense();
			defenseRateText << text[s_DefenseRate] << item->getDefenseRate();
			drawText(renderer, font, defenseText.str(), COLOR_BROWN, xOffset, yOffset + (options.gui.tileHeight * line++), Alignment::Center);
			drawText(renderer, font, defenseRateText.str(), COLOR_BROWN, xOffset, yOffset + (options.gui.tileHeight * line++), Alignment::Center);
		}

		if (item->getDelay() > 0) {
			std::stringstream delayText;
			delayText << text[item->getType() == ItemType::weapon ? s_Delay : s_Speed] << item->getDelay();
			drawText(renderer, font, delayText.str(), COLOR_BROWN, xOffset, yOffset + (options.gui.tileHeight * line++), Alignment::Center);
		}
	}

	drawText(renderer, font, text[s_Def], COLOR_BROWN, options.gui.tabXOffset + 3 * options.gui.tabWidth / 4 - 2 * options.inventory.abilitiesDistance * options.gui.tileWidth, options.gui.tabYOffset + (-1.5f + options.inventory.itemsPerPage) * options.gui.tileHeight, Alignment::Center);
	drawText(renderer, font, text[s_DefRate], COLOR_BROWN, options.gui.tabXOffset + 3 * options.gui.tabWidth / 4 - options.inventory.abilitiesDistance * options.gui.tileWidth, options.gui.tabYOffset + (-1.5f + options.inventory.itemsPerPage) * options.gui.tileHeight, Alignment::Center);
	drawText(renderer, font, text[s_AttMin], COLOR_BROWN, options.gui.tabXOffset + 3 * options.gui.tabWidth / 4, options.gui.tabYOffset + (-1.5f + options.inventory.itemsPerPage) * options.gui.tileHeight, Alignment::Center);
	drawText(renderer, font, text[s_AttMax], COLOR_BROWN, options.gui.tabXOffset + 3 * options.gui.tabWidth / 4 + options.inventory.abilitiesDistance * options.gui.tileWidth, options.gui.tabYOffset + (-1.5f + options.inventory.itemsPerPage) * options.gui.tileHeight, Alignment::Center);
	drawText(renderer, font, text[s_Acc], COLOR_BROWN, options.gui.tabXOffset + 3 * options.gui.tabWidth / 4 + 2 * options.inventory.abilitiesDistance * options.gui.tileWidth, options.gui.tabYOffset + (-1.5f + options.inventory.itemsPerPage) * options.gui.tileHeight, Alignment::Center);

	drawText(renderer, font, STRING(player.creature.getDefense()), COLOR_BROWN, options.gui.tabXOffset + 3 * options.gui.tabWidth / 4 - 2 * options.inventory.abilitiesDistance * options.gui.tileWidth, options.gui.tabYOffset + (-0.5f + options.inventory.itemsPerPage) * options.gui.tileHeight, Alignment::Center);
	drawText(renderer, font, STRING(player.creature.getDefenseRate()), COLOR_BROWN, options.gui.tabXOffset + 3 * options.gui.tabWidth / 4 - options.inventory.abilitiesDistance * options.gui.tileWidth, options.gui.tabYOffset + (-0.5f + options.inventory.itemsPerPage) * options.gui.tileHeight, Alignment::Center);
	drawText(renderer, font, STRING(player.creature.getDamageMin()), COLOR_BROWN, options.gui.tabXOffset + 3 * options.gui.tabWidth / 4, options.gui.tabYOffset + (-0.5f + options.inventory.itemsPerPage) * options.gui.tileHeight, Alignment::Center);
	drawText(renderer, font, STRING(player.creature.getDamageMax()), COLOR_BROWN, options.gui.tabXOffset + 3 * options.gui.tabWidth / 4 + options.inventory.abilitiesDistance * options.gui.tileWidth, options.gui.tabYOffset + (-0.5f + options.inventory.itemsPerPage) * options.gui.tileHeight, Alignment::Center);
	drawText(renderer, font, STRING(player.creature.getAttackRate()), COLOR_BROWN, options.gui.tabXOffset + 3 * options.gui.tabWidth / 4 + 2 * options.inventory.abilitiesDistance * options.gui.tileWidth, options.gui.tabYOffset + (-0.5f + options.inventory.itemsPerPage) * options.gui.tileHeight, Alignment::Center);

	for (size_t abilityIndex = 0; abilityIndex < Ability::count; ++abilityIndex) {
		Ability ability = Ability(abilityIndex);
		drawText(renderer, font, text[ {TextCategory::General, size_t(s_STR) + abilityIndex} ], COLOR_WHITE, options.gui.tabXOffset + options.gui.tabWidth / 2 + (abilityIndex - float(Ability::count - 1) / 2) * options.inventory.abilitiesDistance * options.gui.tileWidth, options.gui.tabYOffset + (0.5f + options.inventory.itemsPerPage) * options.gui.tileHeight, Alignment::Center);
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

void Game::drawStore()
{
	drawInventory();
}

void Game::redrawWorld()
{
	Position playerPosition = player.getPosition();
	int x = clampX(options.gui.centerX - playerPosition.x);
	int y = clampY(options.gui.centerY - playerPosition.y);
	graphics.worldRectangle.x = options.gui.tileBaseWidth * x;
	graphics.worldRectangle.y = options.gui.tileBaseHeight * y;
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

void Game::openStore(NPC* npc)
{
	currentStore = &npc->store;
	activeTab = GUI::Store;
}

void Game::startDialog(NPC* npc)
{
	targetObject = npc;
	currentDialog = &npc->dialog;
	responsePosition = 0;
	dialogID = currentDialog->getStartDialogID(&globalState);
	openTab(GUI::Dialog);
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
							player.setDirection(direction);
							if (currentArea->isPositionFree(player.getPosition() + 1)) {
								player.move(direction);
							}
						}
						break;
					}

					if (keyboard.isKeyPressed(SDLK_LCTRL)) {
						GameObjects objects = currentArea->isPositionTaken(player.getPosition() + 1);
						if (!objects.empty()) {
							GameObject* object = objects[0];
							switch (object->type) {
							case ObjectType::Item: {
								ItemObject* itemObject = (ItemObject*)object;
								if (!player.creature.inventory.isFull()) {
									Item item = itemObject->item;
									player.creature.inventory.addItem(item);
									messages->add(text[s_AddedItem] + text[ {TextCategory::Item, item.getNameID()} ], COLOR_WHITE);
									gameObjects.deleteObject(object);
								} else {
									messages->add(text[s_BackpackIsFull], COLOR_LRED);
								}
								break;
							}
							case ObjectType::Door: {
								Door* door = (Door*)object;
								if (door->toggle()) {
									messages->add(text[s_Closed], door->getColor());
								}
								break;
							}
							case ObjectType::NPC: {
								NPC* npc = (NPC*)object;
								startDialog(npc);
								break;
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
						decrease(inventoryPosition);
					}
					if (keyboard.isKey(SDLK_DOWN) or keyboard.isKey(SDLK_KP_2)) {
						increase(inventoryPosition, player.creature.inventory.getBackpackSize() - 1);
					}
					if (keyboard.isKey(SDLK_LEFT) or keyboard.isKey(SDLK_KP_4) or keyboard.isKey(SDLK_PAGEUP)) {
						subtract(inventoryPosition, options.inventory.itemsPerPage);
					}
					if (keyboard.isKey(SDLK_RIGHT) or keyboard.isKey(SDLK_KP_6) or keyboard.isKey(SDLK_PAGEDOWN)) {
						add(inventoryPosition, options.inventory.itemsPerPage, player.creature.inventory.getBackpackSize() - 1);
					}
					if (keyboard.isKeyPressed(SDLK_RETURN) or keyboard.isKeyPressed(SDLK_KP_ENTER)) {
						if (!player.creature.equipItem(inventoryPosition)) {
							messages->add(text[s_TooHighItemRequirements], COLOR_ORANGE);
						}
					}
				}

				break;
			case GUI::Character:
				if (player.creature.getAbilityPoints() > 0) {
					if (keyboard.isKey(SDLK_UP) or keyboard.isKey(SDLK_KP_8)) {
						decrease(characterInfoPosition);
					}
					if (keyboard.isKey(SDLK_DOWN) or keyboard.isKey(SDLK_KP_2)) {
						increase(characterInfoPosition, (int)(Ability::count) - 1);
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
			case GUI::Dialog:
				if (keyboard.isKey(SDLK_UP) or keyboard.isKey(SDLK_KP_8)) {
					decrease(responsePosition);
				}
				if (keyboard.isKey(SDLK_DOWN) or keyboard.isKey(SDLK_KP_2)) {
					increase(responsePosition, currentDialog->getLine(dialogID).responses.size() - 1);
				}
				/*if (keyboard.isKey(SDLK_LEFT) or keyboard.isKey(SDLK_KP_4) or keyboard.isKey(SDLK_PAGEUP)) {
				    subtract(responsePosition, options.dialog.responsesPerPage);
				}
				if (keyboard.isKey(SDLK_RIGHT) or keyboard.isKey(SDLK_KP_6) or keyboard.isKey(SDLK_PAGEDOWN)) {
				    add(responsePosition, options.inventory.itemsPerPage, player.creature.inventory.getBackpackSize() - 1);
				}*/
				if (keyboard.isKeyPressed(SDLK_RETURN) or keyboard.isKeyPressed(SDLK_KP_ENTER)) {
					Response response = currentDialog->getLineResponse(dialogID, responsePosition);

					globalState.setVariable(response.action.variable, response.action.value);
					dialogID = response.action.nextDialogID;
					switch (dialogID) {
					case ra_OPEN_STORE:
						openStore((NPC*)targetObject);
						break;
					case ra_EXIT:
						activeTab = GUI::None;
						break;
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
			case GUI::Store:
				if (player.creature.inventory.isEmpty()) {
					storeTab = true;
				}

				int& position = (storeTab ? storePosition : inventoryPosition);
				if (keyboard.isKey(SDLK_UP) or keyboard.isKey(SDLK_KP_8)) {
					decrease(position);
				}
				if (keyboard.isKey(SDLK_DOWN) or keyboard.isKey(SDLK_KP_2)) {
					int max = (storeTab ? currentStore->inventory : player.creature.inventory).getBackpackSize() - 1;
					increase(position, max);
				}
				if (keyboard.isKey(SDLK_PAGEUP)) {
					subtract(inventoryPosition, options.inventory.itemsPerPage);
				}
				if (keyboard.isKey(SDLK_PAGEDOWN)) {
					int max = (storeTab ? currentStore->inventory : player.creature.inventory).getBackpackSize() - 1;
					add(inventoryPosition, options.inventory.itemsPerPage, max);
				}
				if (keyboard.isKey(SDLK_LEFT) or keyboard.isKey(SDLK_KP_4)) {
					if (!player.creature.inventory.isEmpty()) {
						storeTab = false;
					}
				}
				if (keyboard.isKey(SDLK_RIGHT) or keyboard.isKey(SDLK_KP_6)) {
					storeTab = true;
				}
				if (keyboard.isKeyPressed(SDLK_RETURN) or keyboard.isKeyPressed(SDLK_KP_ENTER)) {
					if (storeTab) {
						if (!player.creature.inventory.isFull()) {
							Item* item = currentStore->inventory.getBackpackItem(storePosition);
							int price = item->getPrice();
							if (price <= player.creature.getGold()) {
								player.creature.inventory.addItem(*item);
								player.creature.takeGold(price);
								messages->add(text[s_AddedItem] + text[ {TextCategory::Item, item->getNameID()} ], COLOR_WHITE);
							} else {
								messages->add(text[s_NotEnoughGold], COLOR_LRED);
							}
						} else {
							messages->add(text[s_BackpackIsFull], COLOR_LRED);
						}
					} else {
						Item* item = player.creature.inventory.getBackpackItem(inventoryPosition);
						int price = item->getPrice();
						messages->add(text[s_SoldItem] + text[ {TextCategory::Item, item->getNameID()} ], COLOR_WHITE);
						player.creature.inventory.removeItem(inventoryPosition);
						clamp(inventoryPosition, 0, player.creature.inventory.getBackpackSize() - 1);
						player.creature.addGold(price);
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
