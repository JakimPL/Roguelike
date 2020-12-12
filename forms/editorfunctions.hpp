#ifndef EDITORFUNCTIONS_HPP
#define EDITORFUNCTIONS_HPP

#include "src/text.hpp"

#include <QComboBox>
#include <QEvent>
#include <QKeyEvent>

namespace EditorFunctions
{

void globalApplicationSettings(QWidget* widget);
void prepareTextItems(const std::vector<std::string> table, QComboBox* comboBox);
void prepareTextItems(Text* text, const TextCategory category, QComboBox* comboBox);
void prepareTextItems(Text* text, const StringList& pair, QComboBox* comboBox);
void updateTitle(QWidget* widget, const QString title, const QString& path);

}

#endif // EDITORFUNCTIONS_HPP
