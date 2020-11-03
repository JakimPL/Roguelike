#include "itemeditor.hpp"
#include "ui_itemeditor.h"

#include <iostream>
#include <sstream>

#include <QFileDialog>

ItemEditor::ItemEditor(QWidget *parent) : QMainWindow(parent), item("DAGGER"), ui(new Ui::ItemEditor)
{
	ui->setupUi(this);
	updateApplicationTitle();

	for (unsigned int index = 0; index < text.getContentSize(); ++index) {
		QString listItem = QString::fromStdString(text[index]);
		ui->comboBox->insertItem(index, listItem);
	}
}

ItemEditor::~ItemEditor()
{
	delete ui;
}

void ItemEditor::on_actionOpen_triggered()
{
	currentPath = QFileDialog::getOpenFileName(this, tr("Open ITM file"), "./data/ITM/", tr("Item files (*.ITM)"));
	if (currentPath.size() > 0) {
		std::string path = currentPath.toStdString();
		Item newItem(path, true);
		item = newItem;
		updateApplicationTitle();
	}
}

void ItemEditor::on_actionSave_triggered()
{
	if (currentPath.size() > 0) {
		std::string path = currentPath.toStdString();
		item.saveToFile(path, true);
	} else {
		on_actionSaveAs_triggered();
	}
}

void ItemEditor::on_actionSaveAs_triggered()
{
	currentPath = QFileDialog::getSaveFileName(this, tr("Save ITM file"), "./data/ITM/", tr("Item files (*.itm)"));
	std::string path = currentPath.toStdString();
	item.saveToFile(path, true);
	updateApplicationTitle();
}

void ItemEditor::on_actionExit_triggered()
{
	this->close();
}

void ItemEditor::updateApplicationTitle()
{
	if (currentPath.size() > 0) {
		QWidget::setWindowTitle("Item Editor (" + currentPath + ")");
	} else {
		QWidget::setWindowTitle("Item Editor");
	}
}

void ItemEditor::on_comboBox_currentIndexChanged(int index)
{

}
