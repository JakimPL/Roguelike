#ifndef DEFINITIONS_HPP
#define DEFINITIONS_HPP

#include <string>

#define VERSION "0.1.2"
#define DESCRIPTION "Roguelike"
#define PRINT(string) std::cout << string << std::endl

#define GUI_RECTANGLE_COLOR {32,  32,  32,  192}
#define GUI_INVENTORY_COLOR {0,   0,   160, 224}

#define TARGET_COLOR   {64,  128, 224, 255}
#define DOOR_COLOR   {64,  128, 224, 255}

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
#define COLOR_ORANGE  {224, 96,  0,   255}
#define COLOR_PURPLE  {96,  0,   160, 255}
#define COLOR_YGREEN  {192, 255, 32,  255}

#define TILE_EMPTY {'\0', {0, 0, 0}, 0, 0}
#define STARTING_POSITION {30, 30, Direction::SOUTH}

static constexpr unsigned int SIZE_TABLE = 4;
static constexpr unsigned int SIZE_HEADER = 3;
static constexpr unsigned int SIZE_ITEM = 2;
static constexpr unsigned int SIZE_CHAR = sizeof(char);
static constexpr unsigned int SIZE_COLOR = 3 * SIZE_CHAR;
static constexpr unsigned int SIZE_INT = sizeof(unsigned int);
static constexpr unsigned int SIZE_LONG = sizeof(unsigned long);
static constexpr unsigned int SIZE_POSITION = 2 * sizeof(int) + 1;

static constexpr char headerARE[SIZE_HEADER + 1] = {'A', 'R', 'E', '\0'};
static constexpr char headerSTR[SIZE_HEADER + 1] = {'S', 'T', 'R', '\0'};
static constexpr char headerCRE[SIZE_HEADER + 1] = {'C', 'R', 'E', '\0'};
static constexpr char headerITM[SIZE_HEADER + 1] = {'I', 'T', 'M', '\0'};
static constexpr char headerCHR[SIZE_HEADER + 1] = {'C', 'H', 'R', '\0'};

static const std::string PATH_DATA = "data/";
static const std::string PATH_INI = "options.ini";
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

enum class Filetype {
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
