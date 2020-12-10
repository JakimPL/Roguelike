#ifndef DIALOGEDITOR_HPP
#define DIALOGEDITOR_HPP

#include "src/dialog.hpp"
#include "src/text.hpp"

#include <QMainWindow>
#include <QListWidget>

QT_BEGIN_NAMESPACE
namespace Ui
{
class DialogEditor;
}
QT_END_NAMESPACE

class DialogEditor : public QMainWindow
{
	Q_OBJECT

public:
	QString currentPath;
	Text text;
	Dialog dialog;
	DialogEditor(QWidget* parent = nullptr);
	~DialogEditor();

private slots:
	void on_actionOpen_triggered();
	void on_actionSave_triggered();
	void on_actionSaveAs_triggered();
	void on_actionExit_triggered();
	void on_addButton_pressed();
	void on_removeButton_pressed();
	void on_dialogLinesList_itemClicked(QListWidgetItem*);
	void on_textIDBox_currentIndexChanged(int index);

	void prepareEditorValuesAndRanges();
	void updateApplicationTitle();
	void updateDialogParameters();
	void updateDialogLineParameters(unsigned int index);

private:
	Ui::DialogEditor *ui;
};

#endif // DIALOGEDITOR_HPP
