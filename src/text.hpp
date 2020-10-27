#ifndef TEXT_H
#define TEXT_H

#include <string>
#include <vector>

#include "constants.hpp"

#define TEXT(a) (text.text((int)(a)))

class Text
{
private:
	std::vector<std::string> content;
	bool convertFromTXTtoSTR(const std::string &input, const std::string &output);
	bool loadContent(const std::string &filename);

public:
	std::string text(unsigned long id) const;
	Text();
};

static std::vector<Text*> TextCounter;

#endif
