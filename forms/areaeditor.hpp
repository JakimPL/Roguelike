#ifndef AREAEDITOR_HPP
#define AREAEDITOR_HPP

#include "src/area.hpp"
#include "src/text.hpp"

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
	std::vector<QGraphicsTextItem*> textTiles;
	QGraphicsScene* graphicsScene;
	QString currentPath;
	Text text;
	Area area;
	AreaEditor(QWidget *parent = nullptr);
	~AreaEditor();

private slots:
	void on_actionOpen_triggered();
	void on_actionSave_triggered();
	void on_actionSaveAs_triggered();
	void on_actionExit_triggered();

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
