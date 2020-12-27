#ifndef AREAEDITOR_HPP
#define AREAEDITOR_HPP

#include "src/area.hpp"
#include "src/text.hpp"

#include <QKeyEvent>
#include <QMainWindow>
#include <QGraphicsScene>

QT_BEGIN_NAMESPACE
namespace Ui
{

class AreaEditor;
}
QT_END_NAMESPACE

class AreaEditor : public QMainWindow
{
	Q_OBJECT

public:
	GameObjects gameObjects;
	std::map<GameObject*, QGraphicsTextItem*> textObjects;
	std::vector<std::vector<QGraphicsTextItem*>> textTiles;
	std::vector<std::vector<QGraphicsRectItem*>> rectTiles;
	QGraphicsScene* graphicsScene;
	QGraphicsRectItem* selector;
	QString currentPath;
	QFont font;
	Text text;
	Area area;
	Tile currentTile;
	Position selectorPosition;
	AreaEditor(QWidget* parent = nullptr);
	~AreaEditor();

	bool eventFilter(QObject* qObject, QEvent* qEvent);
private slots:
	void on_actionOpen_triggered();
	void on_actionSave_triggered();
	void on_actionSaveAs_triggered();
	void on_actionExit_triggered();
	void on_copyButton_clicked();
	void on_resizeButton_clicked();

	void on_itemResourceBox_editingFinished();
	void on_npcResourceBox_editingFinished();
	void on_dialogResourceBox_editingFinished();
	void on_storeResourceBox_editingFinished();
	void on_allegianceBox_currentIndexChanged(int index);
	void on_doorOrientationBox_stateChanged(int value);
	void on_lockedBox_stateChanged(int value);
	void on_openBox_stateChanged(int value);
	void on_doorRedBox_valueChanged(int value);
	void on_doorGreenBox_valueChanged(int value);
	void on_doorBlueBox_valueChanged(int value);
	void on_dialogIDBox_currentIndexChanged(int index);
	void on_signLetterBox_editingFinished();
	void on_signRedBox_valueChanged(int value);
	void on_signGreenBox_valueChanged(int value);
	void on_signBlueBox_valueChanged(int value);

	GameObject* getCurrentObject();
	Color getColor() const;
	char getLetter(std::string string = "") const;
	void setObject(GameObject* object);
	void setTile(Position position, Tile& tile);

	void drawWorld(bool drawTiles = true);
	void drawObjects();
	void clearEditorElements();
	void prepareEditorElements();
	void prepareEditorValuesAndRanges();
	void setEditorTile();
	void updateApplicationTitle();
	void updateEditorValues();
	void updateAreaParameters();
	void updateObjectEditor();
	void updateObjects();

private:
	Ui::AreaEditor *ui;
};
#endif // AREAEDITOR_HPP
