#include "itemeditor.hpp"
#include "ui_itemeditor.h"

#include <iostream>
#include <sstream>

#include <QFileDialog>

ItemEditor::ItemEditor(QWidget *parent) : QMainWindow(parent), item("DAGGER"), ui(new Ui::ItemEditor)
{
	ui->setupUi(this);

	prepareEditorValuesAndRanges();
	updateApplicationTitle();
	updateEditorValues();
}

ItemEditor::~ItemEditor()
{
	delete ui;
}

void ItemEditor::on_actionOpen_triggered()
{
	currentPath = QFileDialog::getOpenFileName(this, tr("Open ITM file"), "./data/ITM/", tr("Item files (*.ITM)"));
	if (currentPath.size() > 0) {
		std::string path = currentPath.toStdString();
		Item newItem(path, true);
		item = newItem;
		updateApplicationTitle();
		updateEditorValues();
	}
}

void ItemEditor::on_actionSave_triggered()
{
	if (currentPath.size() > 0) {
		std::string path = currentPath.toStdString();
		updateItemParameters();
		item.saveToFile(path, true);
	} else {
		on_actionSaveAs_triggered();
	}
}

void ItemEditor::on_actionSaveAs_triggered()
{
	currentPath = QFileDialog::getSaveFileName(this, tr("Save ITM file"), "./data/ITM/", tr("Item files (*.itm)"));
	std::string path = currentPath.toStdString();
	item.saveToFile(path, true);
	updateApplicationTitle();
}

void ItemEditor::on_actionExit_triggered()
{
	this->close();
}

void ItemEditor::updateApplicationTitle()
{
	if (currentPath.size() > 0) {
		QWidget::setWindowTitle("Item Editor (" + currentPath + ")");
	} else {
		QWidget::setWindowTitle("Item Editor");
	}
}

void ItemEditor::prepareEditorValuesAndRanges()
{
	for (unsigned int index = 0; index < text.getContentSize(); ++index) {
		QString listItem = QString::fromStdString(text[index]);
		ui->textIDBox->insertItem(index, listItem);
		ui->descriptionIDBox->insertItem(index, listItem);
	}

	for (unsigned int index = 0; index < (unsigned int)(ItemType::count); ++index) {
		QString listItem = QString::fromStdString(text[typeTextIDs[index]]);
		ui->typeBox->insertItem(index, listItem);
	}

	for (unsigned int index = 0; index < (unsigned int)(ItemCategory::count); ++index) {
		QString listItem = QString::fromStdString(text[categoryTextIDs[index]]);
		ui->categoryBox->insertItem(index, listItem);
	}

	ui->colorRedBox->setRange(0, 255);
	ui->colorGreenBox->setRange(0, 255);
	ui->colorBlueBox->setRange(0, 255);
}

void ItemEditor::updateEditorValues()
{
	ui->textIDBox->setCurrentIndex(item.getTextID());
	ui->descriptionIDBox->setCurrentIndex(item.getDescriptionID());

	Color color = item.getColor();
	ui->colorRedBox->setValue(color.red);
	ui->colorGreenBox->setValue(color.green);
	ui->colorBlueBox->setValue(color.blue);
	ui->typeBox->setCurrentIndex(int(item.getType()));
	ui->categoryBox->setCurrentIndex(int(item.getType()));
	ui->priceBox->setValue(item.getPrice());
	ui->damageBox->setValue(item.getDamage());
	ui->damageDeltaBox->setValue(item.getDamageDelta());
	ui->attackRateBox->setValue(item.getAttackRate());
	ui->delayBox->setValue(item.getDelay());
	ui->defenseBox->setValue(item.getDefense());
	ui->defenseRateBox->setValue(item.getDefenseRate());
	ui->requiredLevelBox->setValue(item.getRequiredLevel());

	ui->requiredStrengthBox->setValue(item.getRequiredAbility(Ability::strength));
	ui->requiredDexterityBox->setValue(item.getRequiredAbility(Ability::dexterity));
	ui->requiredConstitutionBox->setValue(item.getRequiredAbility(Ability::constitution));
	ui->requiredIntelligenceBox->setValue(item.getRequiredAbility(Ability::intelligence));
	ui->requiredWisdomBox->setValue(item.getRequiredAbility(Ability::wisdom));
}

void ItemEditor::updateItemParameters()
{
	item.setTextID(ui->textIDBox->currentIndex());
	item.setDescriptionID(ui->textIDBox->currentIndex());

	Color color = {(uint8_t)(ui->colorRedBox->value()), (uint8_t)(ui->colorGreenBox->value()), (uint8_t)(ui->colorBlueBox->value())};
	item.setColor(color);
	item.setType(ItemType(ui->typeBox->currentIndex()));
	item.setCategory(ItemCategory(ui->categoryBox->currentIndex()));
	item.setPrice(ui->priceBox->value());
	item.setDamage(ui->damageBox->value());
	item.setDamageDelta(ui->damageDeltaBox->value());
	item.setAttackRate(ui->attackRateBox->value());
	item.setDelay(ui->delayBox->value());
	item.setDefense(ui->defenseBox->value());
	item.setDefenseRate(ui->defenseRateBox->value());
	item.setRequiredLevel(ui->requiredLevelBox->value());

	item.setRequiredAbility(Ability::strength, ui->requiredStrengthBox->value());
	item.setRequiredAbility(Ability::dexterity, ui->requiredDexterityBox->value());
	item.setRequiredAbility(Ability::constitution, ui->requiredConstitutionBox->value());
	item.setRequiredAbility(Ability::intelligence, ui->requiredIntelligenceBox->value());
	item.setRequiredAbility(Ability::wisdom, ui->requiredWisdomBox->value());
}
