#include "areaeditor.hpp"
#include "ui_areaeditor.h"
#include "editorfunctions.hpp"
#include "src/log.hpp"

#include <QFileDialog>
#include <QGraphicsItem>

using namespace EditorFunctions;

AreaEditor::AreaEditor(QWidget *parent)	: QMainWindow(parent), area(), ui(new Ui::AreaEditor)
{
	ui->setupUi(this);

	globalApplicationSettings(this);
	prepareEditorValuesAndRanges();
	updateApplicationTitle();
	prepareEditorElements();
	drawWorld();
}

AreaEditor::~AreaEditor()
{
	_LogNone("Area editor ends");

	clearEditorElements();
	delete ui;
}

void AreaEditor::on_actionOpen_triggered()
{
	currentPath = QFileDialog::getOpenFileName(this, tr("Open ARE file"), "./data/ARE/", tr("Item files (*.ARE)"));
	if (currentPath.size() > 0) {
		std::string path = currentPath.toStdString();
		Area newArea(path, true);
		area = newArea;
		updateApplicationTitle();
		updateEditorValues();
		drawWorld();
	}
}

void AreaEditor::on_actionSave_triggered()
{
	if (currentPath.size() > 0) {
		std::string path = currentPath.toStdString();
		updateAreaParameters();
		area.saveToFile(path, true);
	} else {
		on_actionSaveAs_triggered();
	}
}

void AreaEditor::on_actionSaveAs_triggered()
{
	currentPath = QFileDialog::getSaveFileName(this, tr("Save ARE file"), "./data/ARE/", tr("Area files (*.ARE)"));
	std::string path = currentPath.toStdString();
	area.saveToFile(path, true);
	updateApplicationTitle();
}

void AreaEditor::on_actionExit_triggered()
{
	this->close();
}

void AreaEditor::clearEditorElements()
{
	for (auto pointer : textTiles) {
		delete pointer;
	}

	textTiles.clear();
}

void AreaEditor::drawWorld()
{
	clearEditorElements();

	for (unsigned int y = 0; y < area.getHeight(); ++y) {
		for (unsigned int x = 0; x < area.getWidth(); ++x) {
			QGraphicsTextItem *textItem = new QGraphicsTextItem;
			QFont font = QFont("Courier");
			font.setStyleHint(QFont::Monospace);

			Tile tile = area.getTile(x, y);
			textItem->setDefaultTextColor(tile.color);
			textItem->setPlainText(QString(tile.letter));
			textItem->setPos(x * _TILE_WIDTH, y * _TILE_HEIGHT);
			textItem->setFont(font);

			graphicsScene->addItem(textItem);
			textTiles.push_back(textItem);
		}
	}

}

void AreaEditor::prepareEditorElements()
{
	graphicsScene = new QGraphicsScene(ui->areaView);
	ui->areaView->setScene(graphicsScene);
}

void AreaEditor::prepareEditorValuesAndRanges()
{
	prepareTextItems(&text, TextCategory::Area, ui->nameIDBox);
}

void AreaEditor::updateAreaParameters()
{
	area.setNameID(ui->nameIDBox->currentIndex());
	area.setWidth(ui->widthBox->value());
	area.setHeight(ui->heightBox->value());
}

void AreaEditor::updateEditorValues()
{
	ui->nameIDBox->setCurrentIndex(area.getNameID());
	ui->widthBox->setValue(area.getWidth());
	ui->heightBox->setValue(area.getHeight());
}

void AreaEditor::updateApplicationTitle()
{
	if (currentPath.size() > 0) {
		QWidget::setWindowTitle("Area Editor (" + currentPath + ")");
	} else {
		QWidget::setWindowTitle("Area Editor");
	}
}
