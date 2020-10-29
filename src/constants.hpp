#ifndef DEFINITIONS_HPP
#define DEFINITIONS_HPP

#include <string>

#define GUI_X_OFFSET 8
#define GUI_Y_OFFSET 8
#define GUI_RECTANGLE {16, 16, 16, 224}

#define FONT_SIZE 18
#define TILE_WIDTH 12
#define TILE_HEIGHT 16

#define SIZE_TABLE 4
#define SIZE_INT 4
#define SIZE_HEADER 3
#define SIZE_ITEM 2
#define SIZE_CHAR 1
#define SIZE_COLOR 3

static constexpr char headerARE[SIZE_HEADER + 1] = {'A', 'R', 'E', '\0'};
static constexpr char headerSTR[SIZE_HEADER + 1] = {'S', 'T', 'R', '\0'};
static constexpr char headerCRE[SIZE_HEADER + 1] = {'C', 'R', 'E', '\0'};
static constexpr char headerITM[SIZE_HEADER + 1] = {'I', 'T', 'M', '\0'};
static constexpr char headerCHR[SIZE_HEADER + 1] = {'C', 'H', 'R', '\0'};

static const char PATH_FONT[] = "data/COURIER.TTF";
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

constexpr int SCREEN_WIDTH = 1280;
constexpr int SCREEN_HEIGHT = 768;

enum ErrorCodes {
	OK,
	SDL_ERROR,
	TTF_ERROR,
	FONT_ERROR
};

#endif // DEFINITIONS_HPP
