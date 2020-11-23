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
		Position oldPosition(selectorPosition);
		QKeyEvent* event = static_cast<QKeyEvent*>(qEvent);
		if (event->key() == Qt::Key_Escape) {
			on_actionExit_triggered();
		} else if (event->key() == Qt::Key_Left) {
			selectorPosition.x = std::max(0, selectorPosition.x - 1);
		} else if (event->key() == Qt::Key_Right) {
			selectorPosition.x = std::max(0, std::min(selectorPosition.x + 1, int(area.getWidth()) - 1));
		} else if (event->key() == Qt::Key_Up) {
			selectorPosition.y = std::max(0, selectorPosition.y - 1);
		} else if (event->key() == Qt::Key_Down) {
			selectorPosition.y = std::max(0, std::min(selectorPosition.y + 1, int(area.getHeight()) - 1));
		}

		if (event->key() == Qt::Key_Return) {
			setEditorTile();
			area.setTile(selectorPosition, currentTile);
			setTile(selectorPosition, currentTile);
		} else if (event->key() == Qt::Key_Space) {
			Tile emptyTile = TILE_EMPTY;
			area.setTile(selectorPosition, emptyTile);
			setTile(selectorPosition, emptyTile);
		}

		if (selectorPosition != oldPosition) {
			try {
				Tile tile = area.getTile(selectorPosition);
				std::string objectTypeString = text.text(TextCategory::Object, tile.nameID);
				ui->statusbar->showMessage(QString::fromStdString(objectTypeString));
			}  catch (...) {

			}
		}

		selector->setPos(selectorPosition.x * _TILE_WIDTH, selectorPosition.y * _TILE_HEIGHT);
		return true;
	}

	return QObject::eventFilter(qObject, qEvent);
}

AreaEditor::AreaEditor(QWidget* parent) : QMainWindow(parent), area("MOONDALE"), ui(new Ui::AreaEditor)
{
	ui->setupUi(this);

	globalApplicationSettings(this);
	prepareEditorValuesAndRanges();
	updateApplicationTitle();
	prepareEditorElements();
	updateEditorValues();
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

void AreaEditor::on_copyButton_clicked()
{
	currentTile = area.getTile(selectorPosition);
	ui->objectNameIDBox->setCurrentIndex(currentTile.nameID);
	ui->obstacleBox->setChecked(currentTile.obstacle);

	std::string letterString;
	letterString.push_back(currentTile.letter);
	ui->letterBox->setText(QString::fromStdString(letterString));

	ui->colorRedBox->setValue(currentTile.color.red);
	ui->colorGreenBox->setValue(currentTile.color.green);
	ui->colorBlueBox->setValue(currentTile.color.blue);
}

void AreaEditor::on_resizeButton_clicked()
{
	int width = ui->widthBox->value();
	int height = ui->heightBox->value();

	if (width > 0 and height > 0) {
		area.setDimensions(width, height);
		drawWorld();
	} else {
		_LogError("Area width/height cannot be 0!");
	}
}

void AreaEditor::clearEditorElements()
{
	for (auto vector : textTiles) {
		for (auto pointer : vector) {
			delete pointer;
		}
	}

	for (auto vector : rectTiles) {
		for (auto pointer : vector) {
			delete pointer;
		}
	}

	textTiles.clear();
	rectTiles.clear();
}

Color AreaEditor::getColor() const
{
	Color color = {(uint8_t)(ui->colorRedBox->value()), (uint8_t)(ui->colorGreenBox->value()), (uint8_t)(ui->colorBlueBox->value())};
	return color;
}

char AreaEditor::getLetter(std::string string) const
{
	char letter = '\0';

	if (!string.empty()) {
		letter = string[0];
	}

	return letter;
}

void AreaEditor::drawWorld()
{
	clearEditorElements();

	textTiles.resize(area.getWidth());
	rectTiles.resize(area.getWidth());
	for (unsigned int y = 0; y < area.getHeight(); ++y) {
		for (unsigned int x = 0; x < area.getWidth(); ++x) {
			QGraphicsTextItem* textItem = new QGraphicsTextItem;
			QGraphicsRectItem* rectItem = new QGraphicsRectItem(0, 0, _TILE_WIDTH, _TILE_HEIGHT);
			textTiles[x].push_back(textItem);
			rectTiles[x].push_back(rectItem);

			Position position(x, y);
			Tile tile = area.getTile(position);
			setTile(position, tile);

			graphicsScene->addItem(textItem);
			graphicsScene->addItem(rectItem);
		}
	}

}

void AreaEditor::setTile(Position position, Tile& tile)
{
	textTiles[position.x][position.y]->setDefaultTextColor(tile.color);
	textTiles[position.x][position.y]->setPlainText(QString(tile.letter));
	textTiles[position.x][position.y]->setPos(position.x * _TILE_WIDTH - 2, position.y * _TILE_HEIGHT - 2);
	textTiles[position.x][position.y]->setFont(font);

	QColor qColor;
	rectTiles[position.x][position.y]->setPos(position.x * _TILE_WIDTH, position.y * _TILE_HEIGHT);
	if (tile.obstacle) {
		qColor = Qt::red;
		qColor.setAlphaF(0.5f);
	} else {
		qColor = Qt::black;
		qColor.setAlphaF(0.0f);
	}

	rectTiles[position.x][position.y]->setBrush(qColor);
}

void AreaEditor::prepareEditorElements()
{
	ui->widthBox->setValue(area.getWidth());
	ui->heightBox->setValue(area.getHeight());

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
	setEditorTile();
}

void AreaEditor::setEditorTile()
{
	currentTile.obstacle = ui->obstacleBox->isChecked();
	currentTile.nameID = ui->objectNameIDBox->currentIndex();
	currentTile.letter = getLetter(ui->letterBox->text().toStdString());
	currentTile.color = getColor();
}

void AreaEditor::updateAreaParameters()
{
	area.setNameID(ui->nameIDBox->currentIndex());
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
