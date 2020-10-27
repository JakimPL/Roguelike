#include "functions.hpp"
#include "constants.hpp"
#include "structures.hpp"

#include <cstring>

namespace Functions
{

bool compareHeaders(const char *header1, const char *header2)
{
	return (std::strcmp(header1, header2) == 0);
}

bool loadFont()
{
	/*if (font.loadFromFile(PATH_FONT)) {
		_LogInfo("Font initialized succesfully");
		return true;
	} else {
		std::string errorText = "Failed to load the font file: " + PATH_FONT;
		_LogError(errorText);
	}*/

	return false;
}

}
