#ifndef DEFINITIONS_HPP
#define DEFINITIONS_HPP

#include <string>

#define GUI_X_OFFSET 8
#define GUI_Y_OFFSET 8
#define GUI_RECTANGLE_COLOR {16, 16, 16, 224}

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
#define TILE_WIDTH 12
#define TILE_HEIGHT 22
static const char PATH_FONT[] = "data/COURIER.TTF";
#else
#define FONT_SIZE 16
#define TILE_WIDTH 12
#define TILE_HEIGHT 22
static const char PATH_FONT[] = "data/COURIER.TTF";
#endif

#define SIZE_TABLE 4
#define SIZE_HEADER 3
#define SIZE_ITEM 2
#define SIZE_CHAR 1
#define SIZE_COLOR 3
static constexpr unsigned int SIZE_INT = sizeof(unsigned int);

static constexpr char headerARE[SIZE_HEADER + 1] = {'A', 'R', 'E', '\0'};
static constexpr char headerSTR[SIZE_HEADER + 1] = {'S', 'T', 'R', '\0'};
static constexpr char headerCRE[SIZE_HEADER + 1] = {'C', 'R', 'E', '\0'};
static constexpr char headerITM[SIZE_HEADER + 1] = {'I', 'T', 'M', '\0'};
static constexpr char headerCHR[SIZE_HEADER + 1] = {'C', 'H', 'R', '\0'};

static const std::string FILENAME_PLAYER = "CHARACTER.CRE";
static const std::string FILENAME_STRING = "DIALOGS";
static const std::string PATH_ARE = "data/ARE/";
static const std::string PATH_ITM = "data/ITM/";
static const std::string PATH_CRE = "data/CRE/";
static const std::string PATH_CHR = "data/CHR/";
static const std::string PATH_STR = "data/STR/";
static const std::string ARE_SUFFIX = ".ARE";
static const std::string ITM_SUFFIX = ".ITM";
static const std::string CRE_SUFFIX = ".CRE";
static const std::string CHR_SUFFIX = ".CHR";
static const std::string STR_SUFFIX = ".STR";
static const std::string TXT_SUFFIX = ".TXT";

constexpr int SCREEN_WIDTH = 640;
constexpr int SCREEN_HEIGHT = 480;
constexpr int CENTER_X = SCREEN_WIDTH / (2 * TILE_WIDTH);
constexpr int CENTER_Y = SCREEN_HEIGHT / (2 * TILE_HEIGHT);

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
	Fist
};

#endif // DEFINITIONS_HPP
