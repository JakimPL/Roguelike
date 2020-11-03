#ifndef ITEMEDITOR_HPP
#define ITEMEDITOR_HPP

#include "src/item.hpp"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui
{
class ItemEditor;
}
QT_END_NAMESPACE

class ItemEditor : public QMainWindow
{
	Q_OBJECT

public:
	QString currentPath;
	Text text;
	Item item;
	ItemEditor(QWidget *parent = nullptr);
	~ItemEditor();

private slots:
	void on_actionOpen_triggered();
	void on_actionSave_triggered();
	void on_actionSaveAs_triggered();
	void on_actionExit_triggered();
	void updateApplicationTitle();

	void on_comboBox_currentIndexChanged(int index);

private:
	Ui::ItemEditor *ui;
};
#endif // ITEMEDITOR_HPP
