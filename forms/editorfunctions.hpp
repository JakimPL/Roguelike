#ifndef EDITORFUNCTIONS_HPP
#define EDITORFUNCTIONS_HPP

#include "src/text.hpp"

#include <QComboBox>
#include <QEvent>
#include <QKeyEvent>

namespace EditorFunctions
{

void globalApplicationSettings(QWidget* widget);
void prepareTextItems(Text* text, TextCategory category, QComboBox* comboBox);
void prepareTextItems(Text* text, const StringList& pair, QComboBox* comboBox);

}

#endif // EDITORFUNCTIONS_HPP
