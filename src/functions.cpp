#include "functions.hpp"
#include "color.hpp"
#include "constants.hpp"
#include "log.hpp"
#include "options.hpp"

#include <array>
#include <cstring>
#include <fstream>
#include <sstream>

bpo::options_description Functions::addProgramDescription()
{
	bpo::options_description description(DESCRIPTION);
	description.add_options()
	("help,h",            "Shows the help message")
	("version,v",         "Shows the version of the program")
	("area-editor,a",     "Runs Area Editor")
	("creature-editor,c", "Runs Creature Editor")
	("dialog-editor,d",   "Runs Dialog Editor")
	("item-editor,i",     "Runs Item Editor");

	return description;
}

bool Functions::compareHeaders(const char* header1, const char* header2)
{
	return (std::strcmp(header1, header2) == 0);
}

const std::string Functions::getPath(const std::string& filename, Filetype filetype)
{
	std::string path;
	switch (filetype) {
	case Filetype::ARE:
		path = PATH_DATA + PATH_ARE + filename + SUFFIX_ARE;
		break;
	case Filetype::CHR:
		path = PATH_DATA + PATH_CHR + filename + SUFFIX_CHR;
		break;
	case Filetype::CRE:
		path = PATH_DATA + PATH_CRE + filename + SUFFIX_CRE;
		break;
	case Filetype::DLG:
		path = PATH_DATA + PATH_DLG + filename + SUFFIX_DLG;
		break;
	case Filetype::ITM:
		path = PATH_DATA + PATH_ITM + filename + SUFFIX_ITM;
		break;
	case Filetype::STO:
		path = PATH_DATA + PATH_STO + filename + SUFFIX_STO;
		break;
	case Filetype::STR:
		path = PATH_DATA + PATH_STR + filename + SUFFIX_STR;
		break;
	case Filetype::TXT:
		path = PATH_DATA + PATH_STR + filename + SUFFIX_TXT;
		break;
	default:
		return PATH_DATA + filename;
	}

	return path;
}

bpo::variables_map Functions::getVariablesMap(bpo::options_description description, int argc, char *argv[])
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

std::vector<Mode> Functions::parseProgramArguments(int argc, char *argv[])
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

	if (variablesMap.count("dialog-editor")) {
		modes.push_back(Mode::DialogEditor);
	}

	if (variablesMap.count("item-editor")) {
		modes.push_back(Mode::ItemEditor);
	}

	if (modes.empty()) {
		modes.push_back(Mode::Game);
	}

	return modes;
}

std::vector<std::string> Functions::parseString(const std::string& string, const std::string& delimiter, unsigned int lineWidth)
{
	size_t startPos = 0, endPos;
	size_t delimiterLength = delimiter.length();
	std::string token;
	std::string line;
	std::vector<std::string> lines;

	unsigned int currentWidth = 0;
	while ((endPos = string.find(delimiter, startPos)) != std::string::npos) {
		token = string.substr(startPos, endPos - startPos);
		line += token + delimiter;
		currentWidth += endPos - startPos + delimiterLength;
		startPos = endPos + delimiterLength;

		if (currentWidth > lineWidth) {
			lines.push_back(line);
			line.clear();
			currentWidth = 0;
		}
	}

	line += string.substr(startPos);
	lines.push_back(line);

	return lines;
}

void Functions::read(std::ifstream& resource, char* string, unsigned int size)
{
	resource.read(string, size);
	string[size] = '\0';
}

std::string Functions::readString(std::ifstream& resource, unsigned int size)
{
	char string[size + 1];
	read(resource, string, size);
	return string;

}
