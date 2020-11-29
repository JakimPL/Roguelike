#ifndef OPTIONS_HPP
#define OPTIONS_HPP

#include "constants.hpp"

#include <boost/program_options.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

namespace bpo = boost::program_options;
namespace bpt = boost::property_tree;

#ifdef COURIER_NEW
#define FONT_SIZE 19
#define _TILE_WIDTH 12
#define _TILE_HEIGHT 22
static const char PATH_FONT[] = "data/COURIER.TTF";
#else
#define FONT_SIZE 16
#define _TILE_WIDTH 12
#define _TILE_HEIGHT 22
static const char PATH_FONT[] = "data/NOTOMONO.TTF";
#endif

static constexpr float FPS = 120.0f;
static constexpr float SCALE = 1.5f;
static constexpr bool FULLSCREEN = true;
static constexpr int SCREEN_WIDTH = 1920;
static constexpr int SCREEN_HEIGHT = 1080;

static constexpr unsigned int DELAY = 10;
static constexpr unsigned int KEY_INPUT_DELAY = 8;
static constexpr char TARGET_LETTER = u'¤';
static constexpr char ITEM_LETTER = '?';

static constexpr int GUI_X_OFFSET = 8;
static constexpr int GUI_Y_OFFSET = 8;
static constexpr int TILE_WIDTH = _TILE_WIDTH * SCALE;
static constexpr int TILE_HEIGHT = _TILE_HEIGHT * SCALE;
static constexpr int CENTER_X = SCREEN_WIDTH / (2 * TILE_WIDTH);
static constexpr int CENTER_Y = SCREEN_HEIGHT / (2 * TILE_HEIGHT);
static constexpr int TAB_X_OFFSET = GUI_X_OFFSET + 2 * TILE_WIDTH;
static constexpr int TAB_Y_OFFSET = GUI_Y_OFFSET + 4 * TILE_HEIGHT;
static constexpr int TAB_WIDTH = SCREEN_WIDTH - 2 * GUI_X_OFFSET - 4 * TILE_WIDTH;
static constexpr int TAB_HEIGHT = SCREEN_HEIGHT - 2 * GUI_Y_OFFSET - 8 * TILE_HEIGHT;

static constexpr unsigned int INVENTORY_SIZE = 30;
static constexpr int INVENTORY_ABILITIES_DISTANCE = 6;
static constexpr int INVENTORY_ITEMS_PER_PAGE = (SCREEN_HEIGHT - 2 * GUI_Y_OFFSET - 8 * TILE_HEIGHT) / TILE_HEIGHT - 3;

static constexpr unsigned short MAP_PIXEL_SIZE = 2 * SCALE;
static constexpr unsigned char MAP_ALPHA = 224;

static constexpr float MESSAGE_DURATION = 3.0f;
static constexpr unsigned int MAX_MESSAGES = 6;

/*struct Options {
	struct Global {
		float fps                        = FPS;
		float scale                      = SCALE;
		bool fullscreen                  = FULLSCREEN;
		int screenWidth                  = SCREEN_WIDTH;
		int screenHeight                 = SCREEN_HEIGHT;
	} global;
	struct Game {
		unsigned int delay               = DELAY;
		unsigned int keyInputDelay       = KEY_INPUT_DELAY;
		char targetLetter                = TARGET_LETTER;
		char itemLetter                  = ITEM_LETTER;
	} game;
	struct GUI {
		int guiXOffset                   = GUI_X_OFFSET * SCALE;
		int guiYOffset                   = GUI_Y_OFFSET * SCALE;
		int tileWidth                    = _TILE_WIDTH * SCALE;
		int tileHeight                   = _TILE_WIDTH * SCALE;
		int centerX                      = CENTER_X;
		int centerY                      = CENTER_Y;
		int tabXOffset                   = TAB_X_OFFSET;
		int tabYOffset                   = TAB_Y_OFFSET;
		int tabWidth                     = TAB_WIDTH;
		int tabHeight                    = TAB_HEIGHT;
	} gui;
	struct Inventory {
		unsigned int size                = INVENTORY_SIZE;
		unsigned short abilitiesDistance = INVENTORY_ABILITIES_DISTANCE;
		int itemsPerPage                 = INVENTORY_ITEMS_PER_PAGE;
	} inventory;
	struct Map {
		unsigned short pixelSize         = MAP_PIXEL_SIZE;
		unsigned char alpha              = MAP_ALPHA;
	} map;
	struct Messages {
		float duration                   = MESSAGE_DURATION;
		unsigned int maxMessages         = MAX_MESSAGES;
	} messages;

	void save(const std::string& filename);
	void load(const std::string& filename);
};*/

#endif // OPTIONS_HPP
