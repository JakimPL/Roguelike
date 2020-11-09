#ifndef EDITORFUNCTIONS_HPP
#define EDITORFUNCTIONS_HPP

#include "src/text.hpp"

#include <QComboBox>

namespace EditorFunctions
{
void prepareTextItems(Text* text, TextCategory category, QComboBox* comboBox);
void prepareTextItems(Text* text, const std::pair<TextCategory, std::vector<unsigned int>>& pair, QComboBox* comboBox);
}

#endif // EDITORFUNCTIONS_HPP
