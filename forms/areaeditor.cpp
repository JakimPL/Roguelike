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

		GameObjects objects = area.isPositionTaken(selectorPosition);
		if (event->key() == Qt::Key_Return) {
			setEditorTile();
			area.setTile(selectorPosition, currentTile);
			setTile(selectorPosition, currentTile);
		} else if (event->key() == Qt::Key_Space) {
			Tile emptyTile = TILE_EMPTY;
			area.setTile(selectorPosition, emptyTile);
			setTile(selectorPosition, emptyTile);

			for (GameObject* object : objects) {
				gameObjects.deleteObject(object);
			}
			updateObjects();
		}

		if (objects.empty()) {
			if (event->key() == Qt::Key_D) {
				if (area.getTile(selectorPosition) == Tile(TILE_EMPTY)) {
					GameObjects objects = area.isPositionTaken(selectorPosition);
					if (objects.empty()) {
						new Door(gameObjects, getColor(), false, false, selectorPosition);
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
				creatureName = QInputDialog::getText(this, tr("Input CREATURE resource name:"), tr("Creature name:"), QLineEdit::Normal, "CREATURE", &ok);
				if (ok and !creatureName.isEmpty()) {
					new NPC(gameObjects, creatureName.toStdString(), "", "", selectorPosition, Allegiance::neutral);
					updateObjects();
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
		}

		if (selectorPosition != oldPosition) {
			updateObjectEditor();

			if (objects.empty()) {
				try {
					Tile tile = area.getTile(selectorPosition);
					std::string objectTypeString = text.text(TextCategory::Object, tile.nameID);
					ui->statusbar->showMessage(QString::fromStdString(objectTypeString));
				}  catch (...) {

				}
			} else {
				ui->statusbar->showMessage(QString::fromStdString(text[getCurrentObject()->getText()]));
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
	ui->transparentBox->setChecked(currentTile.transparent);

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


void AreaEditor::on_itemResourceBox_editingFinished()
{
	ItemObject* item = (ItemObject*)(getCurrentObject());
	item->resourceName = ui->itemResourceBox->text().toStdString();
	updateObjectEditor();
}

void AreaEditor::on_npcResourceBox_editingFinished()
{
	NPC* npc = (NPC*)(getCurrentObject());
	npc->setCreatureResourceName(ui->npcResourceBox->text().toStdString());
	updateObjectEditor();
	drawWorld(false);
}

void AreaEditor::on_dialogResourceBox_editingFinished()
{
	NPC* npc = (NPC*)(getCurrentObject());
	npc->setDialogResourceName(ui->dialogResourceBox->text().toStdString());
	updateObjectEditor();
}

void AreaEditor::on_storeResourceBox_editingFinished()
{
	NPC* npc = (NPC*)(getCurrentObject());
	npc->setStoreResourceName(ui->storeResourceBox->text().toStdString());
	updateObjectEditor();
}

void AreaEditor::on_allegianceBox_currentIndexChanged(int index)
{
	NPC* npc = (NPC*)(getCurrentObject());
	if (npc != nullptr) {
		npc->setAllegiance(Allegiance(index));
		updateObjectEditor();
	}
}

void AreaEditor::on_doorOrientationBox_stateChanged(int value)
{
	Door* door = (Door*)(getCurrentObject());
	if (door != nullptr) {
		door->setOrientation(value > 0);

		updateObjectEditor();
		drawWorld(false);
	}
}

void AreaEditor::on_lockedBox_stateChanged(int value)
{
	Door* door = (Door*)(getCurrentObject());
	if (door != nullptr) {
		door->setLocked(value > 0);

		updateObjectEditor();
	}
}

void AreaEditor::on_openBox_stateChanged(int value)
{
	Door* door = (Door*)(getCurrentObject());
	if (door != nullptr) {
		door->setOpen(value > 0);

		updateObjectEditor();
		drawWorld(false);
	}
}

void AreaEditor::on_doorRedBox_valueChanged(int value)
{
	Door* door = (Door*)(getCurrentObject());
	if (door != nullptr) {
		Color color = door->getColor();
		Color newColor = {(unsigned char)(value), color.green, color.blue};
		door->setColor(newColor);
		updateObjectEditor();
		drawWorld(false);
	}
}

void AreaEditor::on_doorGreenBox_valueChanged(int value)
{
	Door* door = (Door*)(getCurrentObject());
	if (door != nullptr) {
		Color color = door->getColor();
		Color newColor = {color.red, (unsigned char)(value), color.blue};
		door->setColor(newColor);
		updateObjectEditor();
		drawWorld(false);
	}
}

void AreaEditor::on_doorBlueBox_valueChanged(int value)
{
	Door* door = (Door*)(getCurrentObject());
	if (door != nullptr) {
		Color color = door->getColor();
		Color newColor = {color.red, color.green, (unsigned char)(value)};
		door->setColor(newColor);
		updateObjectEditor();
		drawWorld(false);
	}
}

void AreaEditor::on_dialogIDBox_currentIndexChanged(int value)
{
	Sign* sign = (Sign*)(getCurrentObject());
	if (sign != nullptr) {
		sign->setNameID(value);
		updateObjectEditor();
	}
}

void AreaEditor::on_signLetterBox_editingFinished()
{
	Sign* sign = (Sign*)(getCurrentObject());
	if (sign != nullptr) {
		sign->setLetter(getLetter(ui->signLetterBox->text().toStdString()));
		updateObjectEditor();
		drawWorld(false);
	}
}

void AreaEditor::on_signRedBox_valueChanged(int value)
{
	Sign* sign = (Sign*)(getCurrentObject());
	if (sign != nullptr) {
		Color color = sign->getColor();
		Color newColor = {(unsigned char)(value), color.green, color.blue};
		sign->setColor(newColor);
		updateObjectEditor();
		drawWorld(false);
	}
}

void AreaEditor::on_signGreenBox_valueChanged(int value)
{
	Sign* sign = (Sign*)(getCurrentObject());
	if (sign != nullptr) {
		Color color = sign->getColor();
		Color newColor = {color.red, (unsigned char)(value), color.blue};
		sign->setColor(newColor);
		updateObjectEditor();
		drawWorld(false);
	}
}

void AreaEditor::on_signBlueBox_valueChanged(int value)
{
	Sign* sign = (Sign*)(getCurrentObject());
	if (sign != nullptr) {
		Color color = sign->getColor();
		Color newColor = {color.red, color.green, (unsigned char)(value)};
		sign->setColor(newColor);
		updateObjectEditor();
		drawWorld(false);
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

GameObject* AreaEditor::getCurrentObject()
{
	GameObjects objects = area.isPositionTaken(selectorPosition);
	if (objects.size() > 0) {
		return objects[0];
	} else {
		return nullptr;
	}
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

void AreaEditor::drawWorld(bool drawTiles)
{
	if (drawTiles) {
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

		drawObjects();
	} else {
		for (auto pair : textObjects) {
			delete pair.second;
		}

		textObjects.clear();
		drawObjects();
	}
}

void AreaEditor::drawObjects()
{
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
	prepareTextItems(&text, TextCategory::Dialog, ui->dialogIDBox);
	prepareTextItems(&text, allegianceNameIDs, ui->allegianceBox);
	setEditorTile();
	updateObjectEditor();
}

void AreaEditor::setEditorTile()
{
	currentTile.obstacle = ui->obstacleBox->isChecked();
	currentTile.transparent = ui->transparentBox->isChecked();
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

void AreaEditor::updateObjectEditor()
{
	GameObject* object = getCurrentObject();

	if (object == nullptr) {
		ui->objectInfoWidget->setCurrentIndex(0);
		ui->objectInfoWidget->setDisabled(true);
	} else {
		ObjectType type = object->type;
		ui->objectInfoWidget->setCurrentIndex(int(object->type) - 1);
		ui->objectInfoWidget->setDisabled(false);
		if (type == ObjectType::Item) {
			ItemObject* item = (ItemObject*)(object);
			ui->itemResourceBox->setText(QString::fromStdString(item->resourceName));
		} else if (type == ObjectType::NPC) {
			NPC* npc = (NPC*)(object);
			ui->npcResourceBox->setText(QString::fromStdString(npc->getCreatureResourceName()));
			ui->dialogResourceBox->setText(QString::fromStdString(npc->getDialogResourceName()));
			ui->storeResourceBox->setText(QString::fromStdString(npc->getStoreResourceName()));
			ui->allegianceBox->setCurrentIndex(int(npc->getAllegiance()));
		} else if (type == ObjectType::Door) {
			Door* door = (Door*)(object);
			ui->doorOrientationBox->setChecked(door->getOrientation());
			ui->lockedBox->setChecked(door->isLocked());
			ui->openBox->setChecked(door->isOpen());

			Color color = door->getColor();
			ui->doorRedBox->setValue(color.red);
			ui->doorGreenBox->setValue(color.green);
			ui->doorBlueBox->setValue(color.blue);
		} else if (type == ObjectType::Sign) {
			Sign* sign = (Sign*)(object);
			ui->dialogIDBox->setCurrentIndex(sign->getNameID());
			ui->signLetterBox->setText(QString::fromStdString({sign->getLetter()}));

			Color color = sign->getColor();
			ui->signRedBox->setValue(color.red);
			ui->signGreenBox->setValue(color.green);
			ui->signBlueBox->setValue(color.blue);
		}
	}
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
