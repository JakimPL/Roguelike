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
	updateEditorValues();
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
		updateEditorValues();
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

void DialogEditor::updateApplicationTitle()
{
	updateTitle(this, "Dialog Editor", currentPath);
}

void DialogEditor::prepareEditorValuesAndRanges()
{
	//prepareTextItems(&text, TextCategory::Item, ui->nameIDBox);
}

void DialogEditor::updateEditorValues()
{

}

void DialogEditor::updateDialogParameters()
{

}
