#include "functions.hpp"
#include "color.hpp"
#include "constants.hpp"
#include "log.hpp"

#include <array>
#include <cstring>
#include <fstream>
#include <sstream>

namespace Functions
{

bpo::options_description addProgramDescription()
{
	bpo::options_description description(DESCRIPTION);
	description.add_options()
	("help,h",            "Shows the help message")
	("version,v",         "Shows the version of the program")
	("area-editor,a",     "Runs Area Editor")
	("creature-editor,c", "Runs Creature Editor")
	("item-editor,i",     "Runs Item Editor");

	return description;
}

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

bpo::variables_map getVariablesMap(bpo::options_description description, int argc, char *argv[])
{
	bpo::variables_map variablesMap;
	bpo::positional_options_description positionalOptionsDescription;
	positionalOptionsDescription.add("help", 0);
	positionalOptionsDescription.add("version", 0);

	try {
		bpo::store(bpo::command_line_parser(argc, argv).options(description).positional(positionalOptionsDescription).run(), variablesMap);
		bpo::notify(variablesMap);
	} catch (std::exception &exception) {
		_LogError("Command option error: " << exception.what());
	} catch (...) {
		_LogError("Unknown error");
	}

	return variablesMap;
}

std::vector<Mode> parseProgramArguments(int argc, char *argv[])
{
	std::vector<Mode> modes;
	bpo::options_description description = addProgramDescription();
	bpo::variables_map variablesMap = getVariablesMap(description, argc, argv);

	if (variablesMap.count("help")) {
		modes.push_back(Mode::Help);
		PRINT(description);
	}

	if (variablesMap.count("version")) {
		PRINT("Version: " << VERSION);
		modes.push_back(Mode::Version);
	}

	if (variablesMap.count("area-editor")) {
		modes.push_back(Mode::AreaEditor);
	}

	if (variablesMap.count("creature-editor")) {
		modes.push_back(Mode::CreatureEditor);
	}

	if (variablesMap.count("item-editor")) {
		modes.push_back(Mode::ItemEditor);
	}

	if (modes.empty()) {
		modes.push_back(Mode::Game);
	}

	return modes;
}

void read(std::ifstream& resource, char* string, unsigned int size)
{
	resource.read(string, size);
	string[size] = '\0';
}

}
