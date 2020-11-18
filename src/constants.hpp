#ifndef DEFINITIONS_HPP
#define DEFINITIONS_HPP

#include <string>

#define VERSION "0.1.1"
#define DESCRIPTION "Roguelike"
#define PRINT(string) std::cout << string << std::endl

#define TEXT_CATEGORY_SIZE 1000000

#define GUI_RECTANGLE_COLOR {32,  32,  32,  192}
#define GUI_INVENTORY_COLOR {0,   0,   160, 224}
#define TARGET_COLOR   {64,  128, 224, 255}

#define COLOR_BLACK   {0,   0,   0,   255}
#define COLOR_RED     {224, 0,   0,   255}
#define COLOR_GREEN   {0,   224, 0,   255}
#define COLOR_BLUE    {0,   0,   224, 255}
#define COLOR_YELLOW  {224, 224, 0,   255}
#define COLOR_MAGENTA {224, 0,   192, 255}
#define COLOR_CYAN    {0,   224, 192, 255}
#define COLOR_WHITE   {255, 255, 255, 255}
#define COLOR_GRAY    {128, 128, 128, 255}
#define COLOR_DGRAY   {64,  64,  64,  255}
#define COLOR_LGRAY   {192, 192, 192, 255}

#define COLOR_DRED    {128, 0,   0,   255}
#define COLOR_DGREEN  {0,   128, 0,   255}
#define COLOR_DBLUE   {0,   0,   128, 255}

#define COLOR_LRED    {255, 64,  64,  255}
#define COLOR_LGREEN  {64,  255, 64,  255}
#define COLOR_LBLUE   {64,  64,  255, 255}

#define COLOR_BROWN   {160, 96,  0,   255}
#define COLOR_ORANGE  {255, 160, 0,   255}
#define COLOR_PURPLE  {96,  0,   160, 255}
#define COLOR_YGREEN  {192, 255, 32,  255}

#define MAP_ALPHA 224

static constexpr unsigned int SIZE_TABLE = 4;
static constexpr unsigned int SIZE_HEADER = 3;
static constexpr unsigned int SIZE_ITEM = 2;
static constexpr unsigned int SIZE_CHAR = 1;
static constexpr unsigned int SIZE_COLOR = 3;
static constexpr unsigned int SIZE_INT = sizeof(unsigned int);
static constexpr unsigned int SIZE_LONG = sizeof(unsigned long);

static constexpr char headerARE[SIZE_HEADER + 1] = {'A', 'R', 'E', '\0'};
static constexpr char headerSTR[SIZE_HEADER + 1] = {'S', 'T', 'R', '\0'};
static constexpr char headerCRE[SIZE_HEADER + 1] = {'C', 'R', 'E', '\0'};
static constexpr char headerITM[SIZE_HEADER + 1] = {'I', 'T', 'M', '\0'};
static constexpr char headerCHR[SIZE_HEADER + 1] = {'C', 'H', 'R', '\0'};

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

static const std::string FILENAME_PLAYER = "CHARACTER.CRE";
static const std::string PATH_DATA = "data/";
static const std::string PATH_ARE = "ARE/";
static const std::string PATH_ITM = "ITM/";
static const std::string PATH_CRE = "CRE/";
static const std::string PATH_CHR = "CHR/";
static const std::string PATH_STR = "STR/";
static const std::string SUFFIX_ARE = ".ARE";
static const std::string SUFFIX_ITM = ".ITM";
static const std::string SUFFIX_CRE = ".CRE";
static const std::string SUFFIX_CHR = ".CHR";
static const std::string SUFFIX_STR = ".STR";
static const std::string SUFFIX_TXT = ".TXT";

static constexpr float FPS = 40.0f;
static constexpr float SCALE = 1.5f;
static constexpr bool FULLSCREEN = true;
static constexpr int GUI_X_OFFSET = 8 * SCALE;
static constexpr int GUI_Y_OFFSET = 8 * SCALE;
static constexpr int SCREEN_WIDTH = 1920;
static constexpr int SCREEN_HEIGHT = 1080;
static constexpr int TILE_WIDTH = _TILE_WIDTH * SCALE;
static constexpr int TILE_HEIGHT = _TILE_HEIGHT * SCALE;
static constexpr int CENTER_X = SCREEN_WIDTH / (2 * TILE_WIDTH);
static constexpr int CENTER_Y = SCREEN_HEIGHT / (2 * TILE_HEIGHT);
static constexpr int TAB_X_OFFSET = GUI_X_OFFSET + 2 * TILE_WIDTH;
static constexpr int TAB_Y_OFFSET = GUI_Y_OFFSET + 4 * TILE_HEIGHT;
static constexpr int TAB_WIDTH = SCREEN_WIDTH - 2 * GUI_X_OFFSET - 4 * TILE_WIDTH;
static constexpr int TAB_HEIGHT = SCREEN_HEIGHT - 2 * GUI_Y_OFFSET - 8 * TILE_HEIGHT;
static constexpr char TARGET_CHAR = u'¤';

static constexpr unsigned int INVENTORY_SIZE = 30;
static constexpr int INVENTORY_ITEMS_PER_PAGE = (SCREEN_HEIGHT - 2 * GUI_Y_OFFSET - 8 * TILE_HEIGHT) / TILE_HEIGHT - 3;
static constexpr int INVENTORY_ABILITIES_DISTANCE = 6;
static constexpr unsigned int KEY_INPUT_DELAY = 8;

static constexpr unsigned short MAP_PIXEL_SIZE = 2 * SCALE;

static constexpr float MESSAGE_DURATION = 3.0f;
static constexpr unsigned int MAX_MESSAGES = 6;

enum Filetype {
	NONE,
	ARE,
	CHR,
	CRE,
	ITM,
	STR,
	TXT
};

enum Alignment {
	Left = 0,
	Center = 1,
	Right = 2,
	Top = 0,
	Bottom = 2
};

enum class Mode {
	Game,
	Help,
	Version,
	AreaEditor,
	CreatureEditor,
	ItemEditor
};

#endif // DEFINITIONS_HPP
