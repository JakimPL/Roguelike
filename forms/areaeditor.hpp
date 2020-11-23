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
	std::vector<std::vector<QGraphicsTextItem*>> textTiles;
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
	void on_resizeButton_clicked();

	Color getColor() const;
	char getLetter(std::string string = "") const;
	void setTile(Position position, Tile& tile);

	void drawWorld();
	void clearEditorElements();
	void prepareEditorElements();
	void prepareEditorValuesAndRanges();
	void updateApplicationTitle();
	void updateEditorValues();
	void updateAreaParameters();

private:
	Ui::AreaEditor *ui;
};
#endif // AREAEDITOR_HPP
