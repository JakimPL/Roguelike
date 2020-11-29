#include "options.hpp"

#include <iostream>

void Options::save(const std::string& filename)
{
	bpt::ptree pt;
	pt.add("general.fps",                 general.fps);
	pt.add("general.scale",               general.scale);
	pt.add("general.fullscreen",          general.fullscreen);
	pt.add("general.screenWidth",         general.screenWidth);
	pt.add("general.screenHeight",        general.screenHeight);

	pt.add("game.delay",                  game.delay);
	pt.add("game.keyInputDelay",          game.keyInputDelay);
	pt.add("game.targetLetter",           game.targetLetter);
	pt.add("game.itemLetter",             game.itemLetter);

	pt.add("gui.guiXOffset",              gui.guiXOffset);
	pt.add("gui.guiYOffset",              gui.guiYOffset);
	pt.add("gui.tileBaseWidth",           gui.tileBaseWidth);
	pt.add("gui.tileBaseHeight",          gui.tileBaseHeight);

	pt.add("inventory.size",              inventory.size);
	pt.add("inventory.abilitiesDistance", inventory.abilitiesDistance);

	pt.add("map.pixelSize",               map.pixelSize);
	pt.add("map.alpha",                   map.alpha);

	pt.add("messages.duration",           messages.duration);
	pt.add("messages.maxMessages",        messages.maxMessages);

	write_ini(filename, pt);
}

void Options::load(const std::string& filename)
{
	bpt::ptree pt;
	read_ini(filename, pt);
	general.fps                 = pt.get<float>("general.fps");
	general.scale               = pt.get<float>("general.scale");
	general.fullscreen          = pt.get<bool>("general.fullscreen");
	general.screenWidth         = pt.get<int>("general.screenWidth");
	general.screenHeight        = pt.get<int>("general.screenHeight");

	game.delay                  = pt.get<unsigned int>("game.delay");
	game.keyInputDelay          = pt.get<unsigned int>("game.keyInputDelay");
	game.targetLetter           = pt.get<char>("game.targetLetter");
	game.itemLetter             = pt.get<char>("game.itemLetter");

	gui.guiXOffset              = pt.get<int>("gui.guiXOffset");
	gui.guiYOffset              = pt.get<int>("gui.guiYOffset");
	gui.tileBaseWidth           = pt.get<int>("gui.tileBaseWidth");
	gui.tileBaseHeight          = pt.get<int>("gui.tileBaseHeight");

	inventory.size              = pt.get<unsigned int>("inventory.size");
	inventory.abilitiesDistance = pt.get<unsigned short>("inventory.abilitiesDistance");

	map.pixelSize               = pt.get<unsigned short>("map.pixelSize");
	map.alpha                   = pt.get<unsigned short>("map.alpha");

	messages.duration           = pt.get<float>("messages.duration");
	messages.maxMessages        = pt.get<unsigned int>("messages.maxMessages");

	gui.tileWidth = gui.tileBaseWidth * general.scale;
	gui.tileHeight = gui.tileBaseHeight * general.scale;
	gui.centerX = general.screenWidth / (2 * gui.tileWidth);
	gui.centerY = general.screenHeight / (2 * gui.tileHeight);
	gui.tabXOffset = gui.guiXOffset + 2 * gui.tileWidth;
	gui.tabYOffset = gui.guiYOffset + 4 * gui.tileHeight;
	gui.tabWidth = general.screenWidth - 2 * gui.guiXOffset - 4 * gui.tileWidth;
	gui.tabHeight = general.screenHeight - 2 * gui.guiYOffset - 8 * gui.tileHeight;

	inventory.itemsPerPage = (general.screenHeight - 2 * gui.guiYOffset - 8 * gui.tabHeight) / gui.tileHeight - 3;
}
