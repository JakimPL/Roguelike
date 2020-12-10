#include "itemeditor.hpp"
#include "ui_itemeditor.h"
#include "editorfunctions.hpp"
#include "src/log.hpp"

#include <QFileDialog>
#include <QLineEdit>

using namespace EditorFunctions;

ItemEditor::ItemEditor(QWidget* parent) : QMainWindow(parent), item(), ui(new Ui::ItemEditor)
{
	ui->setupUi(this);

	globalApplicationSettings(this);
	prepareEditorValuesAndRanges();
	updateApplicationTitle();
	updateEditorValues();
}

ItemEditor::~ItemEditor()
{
	_LogNone("Item editor ends");
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
	currentPath = QFileDialog::getSaveFileName(this, tr("Save ITM file"), "./data/ITM/", tr("Item files (*.ITM)"));
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
	updateTitle(this, "Item Editor", currentPath);
}

void ItemEditor::prepareEditorValuesAndRanges()
{
	/*ui->typeBox->lineEdit()->setAlignment(Qt::AlignRight);
	ui->categoryBox->lineEdit()->setAlignment(Qt::AlignRight);
	ui->flagBox->lineEdit()->setAlignment(Qt::AlignRight);*/
	prepareTextItems(&text, TextCategory::Item, ui->nameIDBox);
	prepareTextItems(&text, TextCategory::Item, ui->descriptionIDBox);
	prepareTextItems(&text, typeNameIDs, ui->typeBox);
	prepareTextItems(&text, categoryNameIDs, ui->categoryBox);
	prepareTextItems(&text, flagNameIDs, ui->flagBox);
}

void ItemEditor::updateEditorValues()
{
	ui->nameIDBox->setCurrentIndex(item.getNameID());
	ui->descriptionIDBox->setCurrentIndex(item.getDescriptionID());

	Color color = item.getColor();
	ui->colorRedBox->setValue(color.red);
	ui->colorGreenBox->setValue(color.green);
	ui->colorBlueBox->setValue(color.blue);
	ui->typeBox->setCurrentIndex(int(item.getType()));
	ui->categoryBox->setCurrentIndex(int(item.getCategory()));
	ui->flagBox->setCurrentIndex(int(item.getFlag()));
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
	item.setNameID(ui->nameIDBox->currentIndex());
	item.setDescriptionID(ui->descriptionIDBox->currentIndex());

	Color color = {(uint8_t)(ui->colorRedBox->value()), (uint8_t)(ui->colorGreenBox->value()), (uint8_t)(ui->colorBlueBox->value())};
	item.setColor(color);
	item.setType(ItemType(ui->typeBox->currentIndex()));
	item.setCategory(ItemCategory(ui->categoryBox->currentIndex()));
	item.setFlag(ItemFlag(ui->flagBox->currentIndex()));
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
