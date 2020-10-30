#include <string>
#include <fstream>
#include <iostream>
#include <string.h>

#include "text.hpp"
#include "constants.hpp"
#include "functions.hpp"
#include "log.hpp"

Text::Text()
{
#ifdef CONVERT_TXT_TO_STR
	convertFromTXTtoSTR(PATH_STR + FILENAME_STRING + TXT_SUFFIX, PATH_STR + FILENAME_STRING + STR_SUFFIX);
#endif
	if (!loadContent(FILENAME_STRING)) {
		std::string errorMessage = "Failed to open " + FILENAME_STRING + " dialog file";
		_LogError(errorMessage);
		throw std::runtime_error(errorMessage);
	}
}

bool Text::convertFromTXTtoSTR(const std::string input, const std::string output)
{
	std::ifstream infile(input);
	std::ofstream outfile(output);
	if (infile.good()) {
		content.clear();
		std::string line;
		while (std::getline(infile, line)) {
			content.push_back(line);
		}
		_LogInfo("Loaded " << input << " file succesfully");
		infile.close();
		if (outfile.good()) {
			outfile.write(headerSTR, SIZE_HEADER);
			unsigned int size = static_cast<unsigned int>(content.size());
			outfile.write(reinterpret_cast<char*>(&size), SIZE_TABLE);
			for (size_t it = 0; it < content.size(); ++it) {
				unsigned short line_size = static_cast<unsigned short>(content[it].size());
				outfile.write(reinterpret_cast<char*>(&line_size), SIZE_ITEM);
			}
			for (size_t it = 0; it < content.size(); ++it) {
				outfile.write(content[it].c_str(), static_cast<unsigned int>(content[it].size()));
			}
			_LogInfo("Saved " << output << " file succesfully");
			outfile.close();
			return true;
		} else {
			_LogError("Failed to save " << output << " file!");
			return false;
		}
	} else {
		_LogError("Failed to load " << input << " file!");
		return false;
	}
}

bool Text::loadContent(const std::string filename)
{
	///TODO: error handling
	std::string path = PATH_STR + filename + STR_SUFFIX;
	_LogInfo("Opening " << path << " dialogs file");
	std::ifstream resource(path);
	if (resource.good()) {
		bool success = true;
		char resourceHeader[SIZE_HEADER + 1];
		Functions::read(resource, resourceHeader, SIZE_HEADER);
		if (Functions::compareHeaders(headerSTR, resourceHeader)) {
			unsigned int tableSize;
			resource.read(reinterpret_cast<char*>(&tableSize), SIZE_TABLE);
			std::vector<short> itemSizes;

			for (unsigned int i = 0; i < tableSize; i++) {
				short string_size;
				resource.read(reinterpret_cast<char*>(&string_size), SIZE_ITEM);
				itemSizes.push_back(string_size);
			}

			if (itemSizes.size() != tableSize) {
				_LogError("Invalid number of items in " << filename << " file!");
				success = false;
			}

			for (unsigned int i = 0; i < tableSize; i++) {
				char item[itemSizes[i] + 1];
				Functions::read(resource, item, itemSizes[i]);
				content.push_back(std::string(item));
			}

			_LogInfo("Dialog file " << filename << " opened successfully");
		} else {
			_LogError("Invalid dialog " << filename << " file!");
		}

		resource.close();
		return success;
	} else {
		return false;
	}
}

const std::string Text::text(unsigned int id) const
{
	std::string output;
	try {
		output = content.at(id);
	}  catch (std::out_of_range &) {
		_LogError("Failed to load a text of id: " << id);
		throw std::runtime_error("failed to load a text");
	}
	return output;
}

const std::string Text::operator[](String id) const
{
	return (*this)[(unsigned int)(id)];
}

const std::string Text::operator[](unsigned int id) const
{
	return text(id);
}
