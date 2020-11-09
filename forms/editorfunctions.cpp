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

void prepareTextItems(Text* text, const std::pair<TextCategory, std::vector<unsigned int>>& pair, QComboBox* comboBox)
{
	for (size_t index = 0; index < pair.second.size(); ++index) {
		QString listItem = QString::fromStdString((*text)[ {pair.first, (unsigned int)(pair.second[index])} ]);
		comboBox->insertItem(index, listItem);
	}
}

}
