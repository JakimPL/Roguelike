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
		if (event->key() == Qt::Key_Escape) {
			on_actionExit_triggered();
		}
		if (event->key() == Qt::Key_Left) {
			selectorPosition.x = std::max(0, selectorPosition.x - 1);
		}
		if (event->key() == Qt::Key_Right) {
			selectorPosition.x = std::max(0, std::min(selectorPosition.x + 1, int(area.getWidth()) - 1));
		}
		if (event->key() == Qt::Key_Up) {
			selectorPosition.y = std::max(0, selectorPosition.y - 1);
		}
		if (event->key() == Qt::Key_Down) {
			selectorPosition.y = std::max(0, std::min(selectorPosition.y + 1, int(area.getHeight()) - 1));
		}
		if (event->key() == Qt::Key_Space or event->key() == Qt::Key_Enter) {
			area.setTile(selectorPosition, currentTile);
			setTile(selectorPosition, currentTile);
		}

		selector->setPos(selectorPosition.x * _TILE_WIDTH, selectorPosition.y * _TILE_HEIGHT);
		return true;
	}

	return QObject::eventFilter(qObject, qEvent);
}

AreaEditor::AreaEditor(QWidget* parent) : QMainWindow(parent), area(), ui(new Ui::AreaEditor)
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
	for (auto vector : textTiles) {
		for (auto pointer : vector) {
			delete pointer;
		}
	}

	textTiles.clear();
}

void AreaEditor::drawWorld()
{
	clearEditorElements();

	textTiles.resize(area.getWidth());
	for (unsigned int y = 0; y < area.getHeight(); ++y) {
		for (unsigned int x = 0; x < area.getWidth(); ++x) {
			QGraphicsTextItem* textItem = new QGraphicsTextItem;
			textTiles[x].push_back(textItem);

			Position position(x, y);
			Tile tile = area.getTile(position);
			setTile(position, tile);

			graphicsScene->addItem(textItem);
		}
	}

}

void AreaEditor::setTile(Position position, Tile& tile)
{
	textTiles[position.x][position.y]->setDefaultTextColor(tile.color);
	textTiles[position.x][position.y]->setPlainText(QString(tile.letter));
	textTiles[position.x][position.y]->setPos(position.x * _TILE_WIDTH, position.y * _TILE_HEIGHT);
	textTiles[position.x][position.y]->setFont(font);
}

void AreaEditor::prepareEditorElements()
{
	selectorPosition.x = 0;
	selectorPosition.y = 0;

	font = QFont("Courier");
	font.setStyleHint(QFont::Monospace);

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
