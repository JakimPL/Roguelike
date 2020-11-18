#include <string>
#include <fstream>
#include <iostream>
#include <string.h>

#include "text.hpp"
#include "constants.hpp"
#include "functions.hpp"
#include "log.hpp"

using namespace Functions;

Text::Text()
{
	content.resize(size_t(TextCategory::Count));
	for (unsigned int category = 0; category < (unsigned int)(TextCategory::Count); ++category) {
#ifdef CONVERT_TXT_TO_STR
		convertFromTXTtoSTR(TextCategory(category), getPath(FILENAME_STRING[category], TXT), getPath(FILENAME_STRING[category], STR));
#else
		if (!loadContent(TextCategory(category), FILENAME_STRING[category])) {
			std::string errorMessage = "Failed to open " + FILENAME_STRING[category] + " dialog file";
			_LogError(errorMessage);
			throw std::runtime_error(errorMessage);
		}
#endif
	}
}

bool Text::convertFromTXTtoSTR(TextCategory category, const std::string& inputPath, const std::string& outputPath)
{
	std::ifstream infile(inputPath);
	std::ofstream outfile(outputPath);
	if (infile.good()) {
		std::vector<std::string>& currentContent = content[size_t(category)];
		currentContent.clear();
		std::string line;
		while (std::getline(infile, line)) {
			currentContent.push_back(line);
		}
		_LogInfo("Loaded " << inputPath << " file succesfully");
		infile.close();
		if (outfile.good()) {
			outfile.write(headerSTR, SIZE_HEADER);
			unsigned int size = static_cast<unsigned int>(currentContent.size());
			outfile.write(reinterpret_cast<char*>(&size), SIZE_TABLE);
			for (size_t it = 0; it < currentContent.size(); ++it) {
				unsigned short line_size = static_cast<unsigned short>(currentContent[it].size());
				outfile.write(reinterpret_cast<char*>(&line_size), SIZE_ITEM);
			}
			for (size_t it = 0; it < currentContent.size(); ++it) {
				outfile.write(currentContent[it].c_str(), static_cast<unsigned int>(currentContent[it].size()));
			}
			_LogInfo("Saved " << outputPath << " file succesfully");
			outfile.close();
			return true;
		} else {
			_LogError("Failed to save " << outputPath << " file!");
			return false;
		}
	} else {
		_LogError("Failed to load " << inputPath << " file!");
		return false;
	}
}

bool Text::loadContent(TextCategory category, const std::string& filename)
{
	///TODO: error handling
	std::string path = getPath(filename, STR);
	_LogInfo("Opening " << path << " dialogs file");
	std::ifstream resource(path);
	if (resource.good()) {
		bool success = true;
		char resourceHeader[SIZE_HEADER + 1];
		read(resource, resourceHeader, SIZE_HEADER);
		if (compareHeaders(headerSTR, resourceHeader)) {
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
				read(resource, item, itemSizes[i]);
				content[size_t(category)].push_back(std::string(item));
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

unsigned int Text::getContentSize(TextCategory category) const
{
	return content[size_t(category)].size();
}

const std::string Text::text(TextCategory category, unsigned int id) const
{
	std::string output;
	try {
		output = content[size_t(category)].at(id);
	}  catch (std::out_of_range &) {
		_LogError("Failed to load a text of id: " << id << " in " << FILENAME_STRING[(unsigned int)(category)]);
		throw std::runtime_error("failed to load a text");
	}

	return output;
}

const std::string Text::operator[](String element)
{
	TextCategory category = TextCategory((unsigned long)(element) / TEXT_CATEGORY_SIZE);
	unsigned int item = (unsigned long)(element) % TEXT_CATEGORY_SIZE;
	return text(category, item);
}

const std::string Text::operator[](std::pair<TextCategory, unsigned int> element) const
{
	return text(element.first, element.second);
}
