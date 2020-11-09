#include "creatureeditor.hpp"
#include "ui_creatureeditor.h"
#include "src/log.hpp"

#include <QFileDialog>

CreatureEditor::CreatureEditor(QWidget *parent)	: QMainWindow(parent), creature(), ui(new Ui::CreatureEditor)
{
	ui->setupUi(this);

	updateApplicationTitle();
}

CreatureEditor::~CreatureEditor()
{
	_LogNone("Creature editor ends");
	delete ui;
}

void CreatureEditor::on_actionOpen_triggered()
{
	currentPath = QFileDialog::getOpenFileName(this, tr("Open CRE file"), "./data/CRE/", tr("Item files (*.CRE)"));
	if (currentPath.size() > 0) {
		std::string path = currentPath.toStdString();
		Creature newCreature(path, true);
		creature = newCreature;
		updateApplicationTitle();
	}
}

void CreatureEditor::on_actionSave_triggered()
{
	if (currentPath.size() > 0) {
		std::string path = currentPath.toStdString();
		creature.saveToFile(path, true);
	} else {
		on_actionSaveAs_triggered();
	}
}

void CreatureEditor::on_actionSaveAs_triggered()
{
	currentPath = QFileDialog::getSaveFileName(this, tr("Save CRE file"), "./data/CRE/", tr("Creature files (*.CRE)"));
	std::string path = currentPath.toStdString();
	creature.saveToFile(path, true);
	updateApplicationTitle();
}

void CreatureEditor::on_actionExit_triggered()
{
	this->close();
}

void CreatureEditor::updateApplicationTitle()
{
	if (currentPath.size() > 0) {
		QWidget::setWindowTitle("Creature Editor (" + currentPath + ")");
	} else {
		QWidget::setWindowTitle("Creature Editor");
	}
}
