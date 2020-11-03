#include "functions.hpp"
#include "constants.hpp"
#include "color.hpp"

#include <array>
#include <cstring>
#include <fstream>
#include <sstream>

namespace Functions
{

bool compareHeaders(const char* header1, const char* header2)
{
	return (std::strcmp(header1, header2) == 0);
}

const std::string getPath(const std::string& filename, Filetype filetype)
{
	std::string path;
	switch (filetype) {
	case ARE:
		path = PATH_DATA + PATH_ARE + filename + SUFFIX_ARE;
		break;
	case CHR:
		path = PATH_DATA + PATH_CHR + filename + SUFFIX_CHR;
		break;
	case CRE:
		path = PATH_DATA + PATH_CRE + filename + SUFFIX_CRE;
		break;
	case ITM:
		path = PATH_DATA + PATH_ITM + filename + SUFFIX_ITM;
		break;
	case STR:
		path = PATH_DATA + PATH_STR + filename + SUFFIX_STR;
		break;
	case TXT:
		path = PATH_DATA + PATH_STR + filename + SUFFIX_TXT;
		break;
	default:
		return PATH_DATA + filename;
	}

	return path;
}

void read(std::ifstream& resource, char* string, unsigned int size)
{
	resource.read(string, size);
	string[size] = '\0';
}

}
