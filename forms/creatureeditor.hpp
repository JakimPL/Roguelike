#ifndef CREATUREEDITOR_HPP
#define CREATUREEDITOR_HPP

#include "src/creature.hpp"
#include "editorfunctions.hpp"
#include <QMainWindow>

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
	CreatureEditor(QWidget *parent = nullptr);
	~CreatureEditor();

private slots:
	void on_actionOpen_triggered();
	void on_actionSave_triggered();
	void on_actionSaveAs_triggered();
	void on_actionExit_triggered();

	void prepareEditorValuesAndRanges();
	void updateApplicationTitle();

private:
	Ui::CreatureEditor *ui;
};

#endif // CREATUREEDITOR_HPP
