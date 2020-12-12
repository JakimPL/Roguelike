#include "editorfunctions.hpp"

namespace EditorFunctions
{

void globalApplicationSettings(QWidget* widget)
{
	const QRect geometry = widget->geometry();
	QSize size = geometry.size();
	widget->setFixedSize(size);
}

void prepareTextItems(const std::vector<std::string> table, QComboBox* comboBox)
{
	for (unsigned int index = 0; index < table.size(); ++index) {
		QString listItem = QString::fromStdString(table[index]);
		comboBox->insertItem(index, listItem);
	}
}

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

void updateTitle(QWidget* widget, const QString title, const QString& path)
{
	if (path.size() > 0) {
		widget->setWindowTitle(title + " (" + path + ")");
	} else {
		widget->setWindowTitle(title);
	}
}

}
