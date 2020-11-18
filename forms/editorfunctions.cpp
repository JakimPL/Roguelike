#include "editorfunctions.hpp"

namespace EditorFunctions
{

void prepareTextItems(Text* text, const TextCategory category, QComboBox* comboBox)
{
	for (unsigned int index = 0; index < text->getContentSize(category); ++index) {
		QString listItem = QString::fromStdString((*text)[ {category, index} ]);
		comboBox->insertItem(index, listItem);
	}
}

void prepareTextItems(Text* text, const StringList& list, QComboBox* comboBox)
{
	for (size_t index = 0; index < list.size(); ++index) {
		QString listItem = QString::fromStdString((*text)[list[index]]);
		comboBox->insertItem(index, listItem);
	}
}

}
