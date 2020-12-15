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
		updateEditorParameters();
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


void DialogEditor::on_startDialogIDBox_valueChanged(int value)
{
	dialog.setStartDialogIDValue(value);
}

void DialogEditor::on_startVariableBox_stateChanged(int value)
{
	dialog.setUseGlobalVariable(value > 0);
	updateDialogParameters();
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

void DialogEditor::on_dialogLinesList_currentRowChanged(int)
{
	ui->responsesList->clear();
	ui->textIDBox->setDisabled(false);
	if (ui->dialogLinesList->count() > 0) {
		DialogLine line = getCurrentLine();
		for (unsigned int responseIndex = 0; responseIndex < line.responsesID.size(); ++responseIndex) {
			unsigned int responseID = getCurrentLine().responsesID[responseIndex];
			DialogResponse response = dialog.getResponse(responseID);
			std::stringstream responseText;
			responseText << responseID << ": " << text[ {TextCategory::Dialog, response.textID} ];
			ui->responsesList->addItem(QString::fromStdString(responseText.str()));
		}

		updateDialogLineParameters(ui->dialogLinesList->currentRow());
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
	if (dialog.getResponsesCount() == 0) {
		dialog.addResponse(DialogResponse());
	}

	getCurrentLine().addResponseID(0);
	int currentIndex = std::max(0, ui->responsesList->currentRow());
	ui->responsesList->addItem("");
	ui->responsesList->setCurrentRow(currentIndex);

	updateResponsesList();
	updateDialogParameters();
}

void DialogEditor::on_removeResponseButton_released()
{
	/*if (!isResponsesListEmpty()) {
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
	}*/
}

void DialogEditor::on_responseIDBox_valueChanged(int value)
{
	if (!isResponsesListEmpty()) {
		if (value == ui->responseIDBox->maximum()) {
			dialog.addResponse(DialogResponse());
		}

		getCurrentLine().setResponseID(ui->responsesList->currentRow(), value);
		updateResponsesList();
		updateResponseParameters();
	}
}

void DialogEditor::on_responseTextIDBox_currentIndexChanged(int index)
{
	if (!isResponsesListEmpty()) {
		getCurrentResponse().textID = index;
		updateResponsesList();
		updateDialogLineParameters(ui->dialogLinesList->currentRow());
	}
}

void DialogEditor::on_responsesList_currentRowChanged(int currentIndex)
{
	if (currentIndex >= 0) {
		updateResponsesList();
		updateResponseParameters();
	}
}

void DialogEditor::on_variableIDBox_valueChanged(int value)
{
	if (!isResponsesListEmpty()) {
		getCurrentResponse().condition.variable = GlobalVariable(value);
		updateResponseParameters();
	}
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
		ui->targetLabel->setText(getCurrentResponse().condition.compareWithVariable ? "Target variable ID:" : "Target value:");
	}
}

void DialogEditor::on_nextDialogIDBox_valueChanged(int value)
{
	if (!isResponsesListEmpty()) {
		getCurrentResponse().action.nextDialog.dialogID = value;
	}
}

void DialogEditor::on_nextDialogFromGVBox_stateChanged(int value)
{
	if (!isResponsesListEmpty()) {
		getCurrentResponse().action.nextDialog.useGlobalVariable = value > 0;
		updateResponseParameters();
	}
}

void DialogEditor::on_globalVariableIDBox_valueChanged(int value)
{
	if (!isResponsesListEmpty()) {
		getCurrentResponse().action.variable = GlobalVariable(value);
		updateResponseParameters();
	}
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

DialogResponse& DialogEditor::getCurrentResponse()
{
	return dialog.getResponseByID(ui->dialogLinesList->currentRow(), ui->responsesList->currentRow());
}

void DialogEditor::updateApplicationTitle()
{
	updateTitle(this, "Dialog Editor", currentPath);
}

void DialogEditor::prepareEditorValuesAndRanges()
{
	ui->dialogIDBox->setDisabled(true);
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

void DialogEditor::updateEditorParameters()
{
	updateDialogLinesList();
	ui->startDialogIDBox->setValue(dialog.getStartDialogIDValue());
	ui->startVariableBox->setChecked(dialog.getUseGlobalVariable());
}

void DialogEditor::updateDialogLinesList()
{
	ui->dialogLinesList->clear();
	for (unsigned int index = 0; index < dialog.getLinesCount(); ++index) {
		ui->dialogLinesList->addItem(QString::fromStdString(text[ {TextCategory::Dialog, dialog.getLine(index).textID} ]));
	}

	if (ui->dialogLinesList->count() > 0) {
		ui->dialogLinesList->setCurrentRow(0);
		if (ui->responsesList->count() > 0) {
			ui->responsesList->setCurrentRow(0);
		}
	}

	updateDialogParameters();
}

void DialogEditor::updateDialogParameters()
{
	for (unsigned int index = 0; index < dialog.getLinesCount(); ++index) {
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

	ui->responseOptionsBox->setDisabled(dialog.getLinesCount() == 0);
	ui->startDialogIDLabel->setText(dialog.getUseGlobalVariable() ? "Global variable ID:" : "Start dialog ID:");
	ui->startDialogIDBox->setMinimum(dialog.getUseGlobalVariable() > 0 ? 1 : 0);
}

void DialogEditor::updateDialogLineParameters(unsigned int index)
{
	DialogLine line = dialog.getLine(index);
	ui->dialogIDBox->setValue(index);
	ui->textIDBox->setCurrentIndex(line.textID);

	for (unsigned int responseID = 0; responseID < line.responsesID.size(); ++responseID) {
		DialogResponse response = dialog.getResponseByID(index, responseID);
		std::string label = text[ {TextCategory::Dialog, response.textID} ];
	}
}

void DialogEditor::updateResponsesList()
{
	ui->responseIDBox->setMaximum(std::max(0, int(dialog.getResponsesCount())));
	if (ui->dialogLinesList->count() > 0) {
		DialogLine line = getCurrentLine();
		for (unsigned int responseIndex = 0; responseIndex < line.responsesID.size(); ++responseIndex) {
			unsigned int responseID = getCurrentLine().responsesID[responseIndex];
			DialogResponse response = dialog.getResponse(responseID);
			std::stringstream responseText;
			responseText << responseID << ": " << text[ {TextCategory::Dialog, response.textID} ];
			setListItem(ui->responsesList, responseIndex, responseText.str());
		}

		updateDialogLineParameters(ui->dialogLinesList->currentRow());
	}
}

void DialogEditor::updateResponseParameters()
{
	if (!isResponsesListEmpty()) {
		DialogResponse response = getCurrentResponse();
		ui->responseIDBox->setValue(getCurrentLine().getResponseID(ui->responsesList->currentRow()));
		ui->responseTextIDBox->setCurrentIndex(response.textID);
		ui->variableIDBox->setValue(response.condition.variable);
		ui->comparisonBox->setCurrentIndex(int(response.condition.comparisonOperator));
		ui->targetBox->setValue(response.condition.value);
		ui->compareValueBox->setChecked(response.condition.compareWithVariable);

		ui->nextDialogIDBox->setValue(response.action.nextDialog.dialogID);
		ui->globalVariableIDBox->setValue(int(response.action.variable));
		ui->setValueBox->setValue(response.action.value);

		ui->targetLabel->setText(response.condition.compareWithVariable ? "Target variable ID:" : "Target value:");
		ui->nextDialogFromGVBox->setChecked(response.action.nextDialog.useGlobalVariable);
		ui->nextDialogIDBox->setMinimum(response.action.nextDialog.useGlobalVariable ? 1 : -2);
		ui->nextDialogIDLabel->setText(response.action.nextDialog.useGlobalVariable ? "Next dialog GV:" : "Next dialog ID:");

		bool conditionDisabled = (ui->variableIDBox->value() <= 0);
		bool actionDisabled = (ui->globalVariableIDBox->value() <= 0);
		ui->compareValueBox->setDisabled(conditionDisabled);
		ui->comparisonBox->setDisabled(conditionDisabled);
		ui->targetBox->setDisabled(conditionDisabled);
		ui->setValueBox->setDisabled(actionDisabled);
	}
}
