#ifndef CREATUREEDITOR_HPP
#define CREATUREEDITOR_HPP

#include "src/creature.hpp"
#include "src/text.hpp"

#include <QMainWindow>
#include <QListWidget>

QT_BEGIN_NAMESPACE
namespace Ui
{
class CreatureEditor;
}
QT_END_NAMESPACE

class CreatureEditor : public QMainWindow
{
	Q_OBJECT

public:
	QString currentPath;
	Text text;
	Creature creature;
	CreatureEditor(QWidget* parent = nullptr);
	~CreatureEditor();

private slots:
	void on_actionOpen_triggered();
	void on_actionSave_triggered();
	void on_actionSaveAs_triggered();
	void on_actionExit_triggered();
	void on_inventoryList_itemDoubleClicked(QListWidgetItem* item);

	void prepareEditorValuesAndRanges();
	void updateApplicationTitle();
	void updateEditorValues();
	void updateCreatureParameters();

private:
	Ui::CreatureEditor *ui;
};

#endif // CREATUREEDITOR_HPP
