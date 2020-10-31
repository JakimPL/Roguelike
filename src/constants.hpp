#ifndef DEFINITIONS_HPP
#define DEFINITIONS_HPP

#include <string>

#define GUI_RECTANGLE_COLOR {16, 16, 16, 224}
#define TARGET_COLOR   {64,  128, 224, 255}

#define COLOR_BLACK   {0,   0,   0,   255}
#define COLOR_RED     {224, 0,   0,   255}
#define COLOR_GREEN   {0,   224, 0,   255}
#define COLOR_BLUE    {0,   0,   224, 255}
#define COLOR_YELLOW  {224, 224, 0,   255}
#define COLOR_MAGENTA {192, 0,   192, 255}
#define COLOR_CYAN    {0,   192, 192, 255}
#define COLOR_WHITE   {252, 255, 255, 255}

#define COLOR_DRED    {160, 0,   0,   255}
#define COLOR_DGREEN  {0,   160, 0,   255}
#define COLOR_DBLUE   {0,   0,   160, 255}

#define COLOR_BROWN   {160, 96,  0,   255}
#define COLOR_PURPLE  {96,  0,   160, 255}

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

#define SIZE_TABLE 4
#define SIZE_HEADER 3
#define SIZE_ITEM 2
#define SIZE_CHAR 1
#define SIZE_COLOR 3

static constexpr unsigned int SIZE_INT = sizeof(unsigned int);
static constexpr unsigned int SIZE_LONG = sizeof(unsigned long);

static constexpr char headerARE[SIZE_HEADER + 1] = {'A', 'R', 'E', '\0'};
static constexpr char headerSTR[SIZE_HEADER + 1] = {'S', 'T', 'R', '\0'};
static constexpr char headerCRE[SIZE_HEADER + 1] = {'C', 'R', 'E', '\0'};
static constexpr char headerITM[SIZE_HEADER + 1] = {'I', 'T', 'M', '\0'};
static constexpr char headerCHR[SIZE_HEADER + 1] = {'C', 'H', 'R', '\0'};

static const std::string FILENAME_PLAYER = "CHARACTER.CRE";
static const std::string FILENAME_STRING = "DIALOGS";
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

static constexpr float SCALE = 2.0f;
static constexpr int GUI_X_OFFSET = 8 * SCALE;
static constexpr int GUI_Y_OFFSET = 8 * SCALE;
static constexpr int SCREEN_WIDTH = 1024;
static constexpr int SCREEN_HEIGHT = 768;
static constexpr int TILE_WIDTH = _TILE_WIDTH * SCALE;
static constexpr int TILE_HEIGHT = _TILE_HEIGHT * SCALE;
static constexpr int CENTER_X = SCREEN_WIDTH / (2 * TILE_WIDTH);
static constexpr int CENTER_Y = SCREEN_HEIGHT / (2 * TILE_HEIGHT);

static constexpr char TARGET_CHAR = u'¤';

static constexpr unsigned int INVENTORY_SIZE = 30;
static constexpr unsigned int INVENTORY_ITEMS_PER_PAGE = 12;

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

enum ErrorCodes {
	OK,
	SDL_ERROR,
	TTF_ERROR,
	FONT_ERROR
};

enum class String : unsigned int {
	empty,
	Roguelike,
	HP,
	MP,
	SHORTCUTS,
	NextLevel,
	Next,
	XP,
	X,
	Y,
	STR,
	DEX,
	CON,
	INT,
	WIS,
	Gold,
	Att,
	Acc,
	level,
	Moondale,
	WeaponInUse,
	Fist,
	Ground,
	Sand,
	Water
};

#endif // DEFINITIONS_HPP
