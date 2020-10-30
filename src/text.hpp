#ifndef TEXT_H
#define TEXT_H

#include <string>
#include <vector>

#include "constants.hpp"

class Text
{
private:
	std::vector<std::string> content;
	bool convertFromTXTtoSTR(const std::string input, const std::string output);
	bool loadContent(const std::string filename);

public:
	Text();
	const std::string text(unsigned int id) const;
	const std::string operator[](String id) const;
	const std::string operator[](unsigned int id) const;
};

static std::vector<Text*> TextCounter;

#endif
