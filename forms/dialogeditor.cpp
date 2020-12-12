#include "dialogeditor.hpp"
#include "ui_dialogeditor.h"
#include "editorfunctions.hpp"
#include "src/log.hpp"

#include <QFileDialog>

using namespace EditorFunctions;

DialogEditor::DialogEditor(QWidget* parent) : QMainWindow(parent), ui(new Ui::DialogEditor)
{
	ui->setupUi(this);

	globalApplicationSettings(this);
	prepareEditorValuesAndRanges();
	updateApplicationTitle();
	updateDialogParameters();
}

DialogEditor::~DialogEditor()
{
	_LogNone("Dialog editor ends");
	delete ui;
}

void DialogEditor::on_actionOpen_triggered()
{
	currentPath = QFileDialog::getOpenFileName(this, tr("Open DLG file"), "./data/DLG/", tr("Dialog files (*.DLG)"));
	if (currentPath.size() > 0) {
		std::string path = currentPath.toStdString();
		Dialog newDialog(path, true);
		dialog = newDialog;
		updateApplicationTitle();
		updateDialogParameters();
	}
}

void DialogEditor::on_actionSave_triggered()
{
	if (currentPath.size() > 0) {
		std::string path = currentPath.toStdString();
		updateDialogParameters();
		dialog.saveToFile(path, true);
	} else {
		on_actionSaveAs_triggered();
	}
}

void DialogEditor::on_actionSaveAs_triggered()
{
	currentPath = QFileDialog::getSaveFileName(this, tr("Save DLG file"), "./data/DLG/", tr("Dialog files (*.DLG)"));
	std::string path = currentPath.toStdString();
	dialog.saveToFile(path, true);
	updateApplicationTitle();
}

void DialogEditor::on_actionExit_triggered()
{
	close();
}

void DialogEditor::on_addButton_pressed()
{
	dialog.addLine(DialogLine());
	ui->dialogLinesList->addItem("");

	int currentIndex = std::max(0, ui->dialogLinesList->currentRow());
	ui->dialogLinesList->setCurrentRow(currentIndex);

	updateDialogParameters();
}

void DialogEditor::on_removeButton_released()
{
	if (ui->dialogLinesList->count() > 0) {
		int currentIndex = ui->dialogLinesList->currentRow();
		dialog.removeLine(currentIndex);
		delete ui->dialogLinesList->takeItem(currentIndex);

		if (ui->dialogLinesList->count() > 0) {
			updateDialogLineParameters(currentIndex);
		} else {
			ui->textIDBox->setCurrentIndex(0);
			ui->dialogIDBox->setValue(0);
			ui->dialogLineOptionsBox->setDisabled(true);
		}

		updateDialogParameters();
	}
}

void DialogEditor::on_dialogLinesList_currentRowChanged(int currentIndex)
{
	ui->responsesList->clearSelection();
	ui->responsesList->clearFocus();
	ui->responsesList->clear();
	ui->textIDBox->setDisabled(false);
	if (ui->dialogLinesList->count() > 0) {
		DialogLine line = dialog.getLine(currentIndex);
		for (unsigned int index = 0; index < line.responses.size(); ++index) {
			Response response = line.responses[index];
			ui->responsesList->addItem(QString::fromStdString(text[ {TextCategory::Dialog, response.textID} ]));
		}

		updateDialogLineParameters(currentIndex);
	}

	updateDialogParameters();
}

void DialogEditor::on_textIDBox_currentIndexChanged(int index)
{
	int currentIndex = ui->dialogLinesList->currentRow();
	if (ui->dialogLinesList->count() > 0) {
		QListWidgetItem* item = ui->dialogLinesList->item(currentIndex);
		item->setText(QString::fromStdString(text[ {TextCategory::Dialog, index} ]));
		dialog.getLine(currentIndex).textID = ui->textIDBox->currentIndex();
	}

	updateDialogParameters();
}

void DialogEditor::on_addResponseButton_pressed()
{
	getCurrentLine().addResponse(Response());
	ui->responsesList->addItem("");

	int currentIndex = std::max(0, ui->responsesList->currentRow());
	ui->responsesList->setCurrentRow(currentIndex);

	updateDialogParameters();
}

void DialogEditor::on_removeResponseButton_released()
{
	if (!isResponsesListEmpty()) {
		int currentIndex = ui->responsesList->currentRow();
		dialog.getLine(currentIndex).removeResponse(currentIndex);
		delete ui->responsesList->takeItem(currentIndex);

		if (!isResponsesListEmpty()) {
			updateResponseParameters(currentIndex);
		} else {
			ui->responsesList->clearSelection();
			ui->responseIDBox->setValue(0);
			ui->responseTextIDBox->setCurrentIndex(0);
			ui->responsesList->setCurrentRow(0);
			ui->responseWidget->setDisabled(true);
		}

		updateDialogParameters();
	}
}

void DialogEditor::on_responseTextIDBox_currentIndexChanged(int index)
{
	if (!isResponsesListEmpty()) {
		int lineIndex = ui->dialogLinesList->currentRow();
		int responseIndex = ui->responsesList->currentRow();

		QListWidgetItem* item = ui->responsesList->item(responseIndex);
		item->setText(QString::fromStdString(text[ {TextCategory::Dialog, index} ]));
		dialog.getLine(lineIndex).responses[responseIndex].textID = index;
		updateDialogLineParameters(lineIndex);
	}
}

void DialogEditor::on_responsesList_currentRowChanged(int currentIndex)
{
	updateResponseParameters(currentIndex);
}

void DialogEditor::on_variableIDBox_valueChanged(int value)
{
	if (!isResponsesListEmpty()) {
		getCurrentResponse().condition.variable = GlobalVariable(value);
	}

	updateResponseParameters(ui->responsesList->currentRow());
}

void DialogEditor::on_comparisonBox_currentIndexChanged(int index)
{
	if (!isResponsesListEmpty()) {
		getCurrentResponse().condition.comparisonOperator = ComparisonOperator(index);
	}
}

void DialogEditor::on_targetBox_valueChanged(int value)
{
	if (!isResponsesListEmpty()) {
		getCurrentResponse().condition.value = value;
	}
}

void DialogEditor::on_compareValueBox_stateChanged(int value)
{
	if (!isResponsesListEmpty()) {
		getCurrentResponse().condition.compareWithVariable = value > 0;
	}

	ui->targetLabel->setText(getCurrentResponse().condition.compareWithVariable ? "Target variable ID:" : "Target value:");
}

void DialogEditor::on_nextDialogIDBox_valueChanged(int value)
{
	if (!isResponsesListEmpty()) {
		getCurrentResponse().action.nextDialogID = value;
	}
}

void DialogEditor::on_globalVariableIDBox_valueChanged(int value)
{
	if (!isResponsesListEmpty()) {
		getCurrentResponse().action.variable = GlobalVariable(value);
	}

	updateResponseParameters(ui->responsesList->currentRow());
}

void DialogEditor::on_setValueBox_valueChanged(int value)
{
	if (!isResponsesListEmpty()) {
		getCurrentResponse().action.value = value;
	}
}

bool DialogEditor::isResponsesListEmpty() const
{
	return (ui->responsesList->count() == 0);
}

DialogLine& DialogEditor::getCurrentLine()
{
	return dialog.getLine(ui->dialogLinesList->currentRow());
}

Response& DialogEditor::getCurrentResponse()
{
	return getCurrentLine().responses[ui->responsesList->currentRow()];
}

void DialogEditor::updateApplicationTitle()
{
	updateTitle(this, "Dialog Editor", currentPath);
}

void DialogEditor::prepareEditorValuesAndRanges()
{
	ui->dialogIDBox->setDisabled(true);
	ui->responseIDBox->setDisabled(true);
	prepareTextItems(&text, TextCategory::Dialog, ui->textIDBox);
	prepareTextItems(&text, TextCategory::Dialog, ui->responseTextIDBox);
	prepareTextItems(OPERATOR_STRING, ui->comparisonBox);
}

void DialogEditor::setListItem(QListWidget* widget, unsigned int index, const std::string& label)
{
	QListWidgetItem* listItem = widget->item(index);
	QFont listItemFont = listItem->font();

	if (label.empty()) {
		listItem->setText("empty");
		listItemFont.setWeight(QFont::Cursive);
	} else {
		listItem->setText(QString::fromStdString(label));
		listItemFont.setWeight(QFont::Normal);
	}

	listItem->setFont(listItemFont);
}

void DialogEditor::updateDialogParameters()
{
	for (unsigned int index = 0; index < dialog.getSize(); ++index) {
		DialogLine line = dialog.getLine(index);
		std::string label = text[ {TextCategory::Dialog, line.textID} ];
		setListItem(ui->dialogLinesList, index, label);
	}

	if (ui->dialogLinesList->count() > 0) {
		int currentIndex = ui->dialogLinesList->currentRow();
		updateDialogLineParameters(currentIndex);
		ui->dialogLineOptionsBox->setDisabled(false);
	} else {
		ui->dialogLineOptionsBox->setDisabled(true);
	}

	ui->responseOptionsBox->setDisabled(ui->responsesList->count() == 0);
}

void DialogEditor::updateDialogLineParameters(unsigned int index)
{
	DialogLine line = dialog.getLine(index);
	ui->dialogIDBox->setValue(index);
	ui->textIDBox->setCurrentIndex(line.textID);

	for (unsigned int index = 0; index < line.responses.size(); ++index) {
		Response response = line.responses[index];
		std::string label = text[ {TextCategory::Dialog, response.textID} ];
		setListItem(ui->responsesList, index, label);
	}
}

void DialogEditor::updateResponseParameters(unsigned int index)
{
	DialogLine line = getCurrentLine();
	ui->responseIDBox->setValue(index);
	ui->responseTextIDBox->setCurrentIndex(line.responses[index].textID);
	ui->variableIDBox->setValue(line.responses[index].condition.variable);
	ui->comparisonBox->setCurrentIndex(int(line.responses[index].condition.comparisonOperator));
	ui->targetBox->setValue(line.responses[index].condition.value);
	ui->compareValueBox->setChecked(line.responses[index].condition.compareWithVariable);

	ui->nextDialogIDBox->setValue(line.responses[index].action.nextDialogID);
	ui->globalVariableIDBox->setValue(int(line.responses[index].action.variable));
	ui->setValueBox->setValue(line.responses[index].action.value);

	ui->targetLabel->setText(line.responses[index].condition.compareWithVariable ? "Target variable ID:" : "Target value:");

	bool conditionDisabled = (ui->variableIDBox->value() <= 0);
	bool actionDisabled = (ui->globalVariableIDBox->value() <= 0);
	ui->compareValueBox->setDisabled(conditionDisabled);
	ui->comparisonBox->setDisabled(conditionDisabled);
	ui->targetBox->setDisabled(conditionDisabled);
	ui->setValueBox->setDisabled(actionDisabled);
}
