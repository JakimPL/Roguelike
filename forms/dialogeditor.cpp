#include "dialogeditor.hpp"
#include "ui_dialogeditor.h"
#include "editorfunctions.hpp"
#include "src/log.hpp"

#include <QFileDialog>

using namespace EditorFunctions;

DialogEditor::DialogEditor(QWidget* parent) : QMainWindow(parent), ui(new Ui::DialogEditor)
{
	ui->setupUi(this);

	globalApplicationSettings(this);
	prepareEditorValuesAndRanges();
	updateApplicationTitle();
	updateDialogParameters();
}

DialogEditor::~DialogEditor()
{
	_LogNone("Dialog editor ends");
	delete ui;
}

void DialogEditor::on_actionOpen_triggered()
{
	currentPath = QFileDialog::getOpenFileName(this, tr("Open DLG file"), "./data/DLG/", tr("Dialog files (*.DLG)"));
	if (currentPath.size() > 0) {
		std::string path = currentPath.toStdString();
		Dialog newDialog(path, true);
		dialog = newDialog;
		updateApplicationTitle();
		updateDialogParameters();
	}
}

void DialogEditor::on_actionSave_triggered()
{
	if (currentPath.size() > 0) {
		std::string path = currentPath.toStdString();
		updateDialogParameters();
		dialog.saveToFile(path, true);
	} else {
		on_actionSaveAs_triggered();
	}
}

void DialogEditor::on_actionSaveAs_triggered()
{
	currentPath = QFileDialog::getSaveFileName(this, tr("Save DLG file"), "./data/DLG/", tr("Dialog files (*.DLG)"));
	std::string path = currentPath.toStdString();
	dialog.saveToFile(path, true);
	updateApplicationTitle();
}

void DialogEditor::on_actionExit_triggered()
{
	this->close();
}

void DialogEditor::on_addButton_pressed()
{
	dialog.addLine(DialogLine());
	ui->dialogLinesList->addItem("");
	updateDialogParameters();
}

void DialogEditor::on_removeButton_pressed()
{
	if (ui->dialogLinesList->count() > 0) {
		dialog.removeLine(ui->dialogLinesList->currentIndex().row());
		ui->dialogLinesList->takeItem(ui->dialogLinesList->currentIndex().row());

		if (ui->dialogLinesList->count() != 0) {
			updateDialogLineParameters(ui->dialogLinesList->currentIndex().row());
		} else {
			ui->textIDBox->setCurrentIndex(0);
			ui->textIDBox->setDisabled(false);
		}

		updateDialogParameters();
	}
}

void DialogEditor::on_dialogLinesList_itemClicked(QListWidgetItem*)
{
	int index = ui->dialogLinesList->currentIndex().row();
	updateDialogLineParameters(index);
	ui->textIDBox->setDisabled(false);
}

void DialogEditor::on_textIDBox_currentIndexChanged(int index)
{
	int currentIndex = ui->dialogLinesList->currentIndex().row();
	if (ui->dialogLinesList->count() != 0) {
		QListWidgetItem* item = ui->dialogLinesList->item(currentIndex);
		item->setText(QString::fromStdString(text[ {TextCategory::Dialog, index} ]));
		dialog.getLine(currentIndex).textID = ui->textIDBox->currentIndex();
	}

	updateDialogParameters();
}

void DialogEditor::updateApplicationTitle()
{
	updateTitle(this, "Dialog Editor", currentPath);
}

void DialogEditor::prepareEditorValuesAndRanges()
{
	ui->dialogIDBox->setDisabled(true);
	prepareTextItems(&text, TextCategory::Dialog, ui->textIDBox);
}

void DialogEditor::updateDialogParameters()
{
	for (unsigned int index = 0; index < dialog.getSize(); ++index) {
		DialogLine dialogLine = dialog.getLine(index);
		std::string label = text[ {TextCategory::Dialog, dialogLine.textID} ];

		QListWidgetItem* listItem = ui->dialogLinesList->item(index);
		QFont listItemFont = listItem->font();

		if (label.empty()) {
			listItem->setText("empty");
			listItemFont.setWeight(QFont::Cursive);
		} else {
			listItem->setText(QString::fromStdString(label));
			listItemFont.setWeight(QFont::Normal);
		}

		listItem->setFont(listItemFont);
	}

	if (ui->dialogLinesList->count() == 0) {
		ui->textIDBox->setDisabled(true);
	}
}

void DialogEditor::updateDialogLineParameters(unsigned int index)
{
	DialogLine line = dialog.getLine(index);
	ui->dialogIDBox->setValue(index);
	ui->textIDBox->setCurrentIndex(line.textID);
}
