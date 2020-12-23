#include "areaeditor.hpp"
#include "ui_areaeditor.h"
#include "editorfunctions.hpp"
#include "src/log.hpp"
#include "src/options.hpp"

#include "src/objects/door.hpp"
#include "src/objects/itemobject.hpp"
#include "src/objects/npc.hpp"
#include "src/objects/sign.hpp"

#include <QInputDialog>
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

			GameObjects objects = area.isPositionTaken(selectorPosition);
			for (GameObject* object : objects) {
				gameObjects.deleteObject(object);
			}
			updateObjects();
		} else if (event->key() == Qt::Key_D) {
			if (area.getTile(selectorPosition) == Tile(TILE_EMPTY)) {
				GameObjects objects = area.isPositionTaken(selectorPosition);
				if (objects.empty()) {
					new Door(gameObjects, getColor(), false, false, selectorPosition);
					updateObjects();
				} else if (objects[0]->type == ObjectType::Door) {
					Door* door = (Door*)(objects[0]);
					door->setOrientation(1 - door->getOrientation());
					updateObjects();
				}
			}
		} else if (event->key() == Qt::Key_I) {
			bool ok;
			QString resourceName = QInputDialog::getText(this, tr("Input ITEM resource name:"), tr("Item name:"), QLineEdit::Normal, "ITEM", &ok);
			if (ok and !resourceName.isEmpty()) {
				new ItemObject(gameObjects, resourceName.toStdString(), selectorPosition);
				updateObjects();
			}
		} else if (event->key() == Qt::Key_N) {
			bool ok;
			QString creatureName;
			QString dialogName;
			QString storeName;
			creatureName = QInputDialog::getText(this, tr("Input CREATURE resource name:"), tr("Creature name:"), QLineEdit::Normal, "CREATURE", &ok);
			if (ok) {
				dialogName = QInputDialog::getText(this, tr("Input DIALOG resource name:"), tr("Dialog name:"), QLineEdit::Normal, "DIALOG", &ok);
				storeName = QInputDialog::getText(this, tr("Input STORE resource name:"), tr("Store name:"), QLineEdit::Normal, "STORE", &ok);
				if (ok and !dialogName.isEmpty()) {
					new NPC(gameObjects, creatureName.toStdString(), dialogName.toStdString(), storeName.toStdString(), selectorPosition);
					updateObjects();
				}
			}
		} else if (event->key() == Qt::Key_S) {
			bool ok;
			QString nameIDText = QInputDialog::getText(this, tr("Input name ID:"), tr("Name ID:"), QLineEdit::Normal, "0", &ok);
			if (ok and !nameIDText.isEmpty()) {
				int nameID = QString::fromStdString(nameIDText.toStdString()).toInt(&ok);
				if (ok) {
					new Sign(gameObjects, getColor(), selectorPosition, getLetter(ui->letterBox->text().toStdString()), nameID);
					updateObjects();
				} else {
					_LogError("Bad input!");
				}
			}
		}

		if (selectorPosition != oldPosition) {
			try {
				Tile tile = area.getTile(selectorPosition);
				std::string objectTypeString = text.text(TextCategory::Object, tile.nameID);
				ui->statusbar->showMessage(QString::fromStdString(objectTypeString));
			}  catch (...) {

			}
		}

		selector->setPos(selectorPosition.x * options.gui.tileBaseWidth, selectorPosition.y * options.gui.tileBaseHeight);
		return true;
	}

	return QObject::eventFilter(qObject, qEvent);
}

AreaEditor::AreaEditor(QWidget* parent) : QMainWindow(parent), area(gameObjects, "MOONDALE"), ui(new Ui::AreaEditor)
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
		area.loadFromFile(path, true);
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
	close();
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
	for (auto pair : textObjects) {
		delete pair.second;
	}

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

	textObjects.clear();
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
			QGraphicsRectItem* rectItem = new QGraphicsRectItem(0, 0, options.gui.tileBaseWidth, options.gui.tileBaseHeight);
			textTiles[x].push_back(textItem);
			rectTiles[x].push_back(rectItem);

			Position position(x, y);
			Tile tile = area.getTile(position);
			setTile(position, tile);

			graphicsScene->addItem(textItem);
			graphicsScene->addItem(rectItem);
		}
	}

	for (GameObject* object : gameObjects) {
		QGraphicsTextItem* textItem = new QGraphicsTextItem;
		textObjects[object] = textItem;
		setObject(object);
		graphicsScene->addItem(textItem);
	}
}

void AreaEditor::setObject(GameObject* object)
{
	Position position = object->getPosition();
	textObjects[object]->setDefaultTextColor(object->getColor());
	textObjects[object]->setPlainText(QString(object->getLetter()));
	textObjects[object]->setPos(position.x * options.gui.tileBaseWidth - 2, position.y * options.gui.tileBaseHeight - 2);
	textObjects[object]->setFont(font);
}

void AreaEditor::setTile(Position position, Tile& tile)
{
	textTiles[position.x][position.y]->setDefaultTextColor(tile.color);
	textTiles[position.x][position.y]->setPlainText(QString(tile.letter));
	textTiles[position.x][position.y]->setPos(position.x * options.gui.tileBaseWidth - 2, position.y * options.gui.tileBaseHeight - 2);
	textTiles[position.x][position.y]->setFont(font);

	QColor qColor;
	rectTiles[position.x][position.y]->setPos(position.x * options.gui.tileBaseWidth, position.y * options.gui.tileBaseHeight);
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

	selector = new QGraphicsRectItem(0, 0, options.gui.tileBaseWidth, options.gui.tileBaseHeight);
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
	updateTitle(this, "Area Editor", currentPath);
}

void AreaEditor::updateObjects()
{
	for (auto pair : textObjects) {
		delete pair.second;
	}

	textObjects.clear();

	for (GameObject* object : gameObjects) {
		QGraphicsTextItem* textItem = new QGraphicsTextItem;
		textObjects[object] = textItem;
		setObject(object);
		graphicsScene->addItem(textItem);
	}
}
