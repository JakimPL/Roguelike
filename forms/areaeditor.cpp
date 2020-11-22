#include "areaeditor.hpp"
#include "ui_areaeditor.h"
#include "editorfunctions.hpp"
#include "src/log.hpp"

#include <QFileDialog>
#include <QGraphicsItem>

using namespace EditorFunctions;

bool AreaEditor::eventFilter(QObject* qObject, QEvent* qEvent)
{
	if (qEvent->type() == QEvent::KeyPress) {
		QKeyEvent* event = static_cast<QKeyEvent*>(qEvent);
		if (event->key() == Qt::Key_Left) {

		}
		if (event->key() == Qt::Key_Right) {

		}
		if (event->key() == Qt::Key_Up) {

		}
		if (event->key() == Qt::Key_Down) {

		}

		return true;
	}

	return QObject::eventFilter(qObject, qEvent);
}

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
	delete selector;
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
	selectorPosition.x = 0;
	selectorPosition.y = 0;

	selector = new QGraphicsRectItem(0, 0, _TILE_WIDTH, _TILE_HEIGHT);
	selector->setBrush(Qt::yellow);

	graphicsScene = new QGraphicsScene(ui->areaView);
	graphicsScene->addItem(selector);
	graphicsScene->setFocus();
	ui->areaView->setScene(graphicsScene);

	installEventFilter(this);
	graphicsScene->installEventFilter(this);
}

void AreaEditor::prepareEditorValuesAndRanges()
{
	prepareTextItems(&text, TextCategory::Area, ui->nameIDBox);
	prepareTextItems(&text, TextCategory::Object, ui->objectNameIDBox);

	std::string letter = ui->letterBox->text().toStdString();

	if (letter.size() > 0) {
		currentTile.letter = letter[0];
	} else {
		currentTile.letter = '\0';
	}
	currentTile.color = {0, 255, 128};
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
