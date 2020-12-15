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

	void on_startDialogIDBox_valueChanged(int value);
	void on_startVariableBox_stateChanged(int value);

	void on_addButton_pressed();
	void on_removeButton_released();
	void on_dialogLinesList_currentRowChanged(int);
	void on_textIDBox_currentIndexChanged(int index);

	void on_addResponseButton_pressed();
	void on_removeResponseButton_released();
	void on_responsesList_currentRowChanged(int currentIndex);

	void on_responseIDBox_valueChanged(int value);
	void on_responseTextIDBox_currentIndexChanged(int index);

	void on_variableIDBox_valueChanged(int value);
	void on_comparisonBox_currentIndexChanged(int index);
	void on_targetBox_valueChanged(int value);
	void on_compareValueBox_stateChanged(int value);
	void on_nextDialogIDBox_valueChanged(int value);
	void on_nextDialogFromGVBox_stateChanged(int value);
	void on_globalVariableIDBox_valueChanged(int value);
	void on_setValueBox_valueChanged(int value);

	bool isResponsesListEmpty() const;
	DialogLine& getCurrentLine();
	DialogResponse& getCurrentResponse();

	void prepareEditorValuesAndRanges();
	void setListItem(QListWidget* widget, unsigned int index, const std::string& label);
	void updateApplicationTitle();
	void updateEditorParameters();
	void updateDialogLinesList();
	void updateDialogParameters();
	void updateDialogLineParameters(unsigned int index);
	void updateResponsesList();
	void updateResponseParameters();

private:
	Ui::DialogEditor *ui;
};

#endif // DIALOGEDITOR_HPP
