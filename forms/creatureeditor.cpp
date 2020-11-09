#include "creatureeditor.hpp"
#include "ui_creatureeditor.h"
#include "src/log.hpp"

#include <QFileDialog>

using namespace EditorFunctions;

CreatureEditor::CreatureEditor(QWidget *parent)	: QMainWindow(parent), creature(), ui(new Ui::CreatureEditor)
{
	ui->setupUi(this);

	prepareEditorValuesAndRanges();
	updateApplicationTitle();
	updateEditorValues();
}

CreatureEditor::~CreatureEditor()
{
	_LogNone("Creature editor ends");
	delete ui;
}

void CreatureEditor::prepareEditorValuesAndRanges()
{
	ui->letterBox->setAlignment(Qt::AlignRight);
	/*ui->raceBox->lineEdit()->setAlignment(Qt::AlignRight);
	ui->genderBox->lineEdit()->setAlignment(Qt::AlignRight);
	ui->alignmentBox->lineEdit()->setAlignment(Qt::AlignRight);
	ui->stateBox->lineEdit()->setAlignment(Qt::AlignRight);*/
	prepareTextItems(&text, TextCategory::Creature, ui->nameIDBox);
	prepareTextItems(&text, raceNameIDs, ui->raceBox);
	prepareTextItems(&text, genderNameIDs, ui->genderBox);
	prepareTextItems(&text, alignmentNameIDs, ui->alignmentBox);
	prepareTextItems(&text, stateNameIDs, ui->stateBox);
}

void CreatureEditor::on_actionOpen_triggered()
{
	currentPath = QFileDialog::getOpenFileName(this, tr("Open CRE file"), "./data/CRE/", tr("Item files (*.CRE)"));
	if (currentPath.size() > 0) {
		std::string path = currentPath.toStdString();
		Creature newCreature(path, true);
		creature = newCreature;
		updateApplicationTitle();
		updateEditorValues();
	}
}

void CreatureEditor::on_actionSave_triggered()
{
	if (currentPath.size() > 0) {
		std::string path = currentPath.toStdString();
		creature.saveToFile(path, true);
	} else {
		on_actionSaveAs_triggered();
	}
}

void CreatureEditor::on_actionSaveAs_triggered()
{
	currentPath = QFileDialog::getSaveFileName(this, tr("Save CRE file"), "./data/CRE/", tr("Creature files (*.CRE)"));
	std::string path = currentPath.toStdString();
	creature.saveToFile(path, true);
	updateApplicationTitle();
}

void CreatureEditor::on_actionExit_triggered()
{
	this->close();
}

void CreatureEditor::updateApplicationTitle()
{
	if (currentPath.size() > 0) {
		QWidget::setWindowTitle("Creature Editor (" + currentPath + ")");
	} else {
		QWidget::setWindowTitle("Creature Editor");
	}
}

void CreatureEditor::updateEditorValues()
{
	ui->nameIDBox->setCurrentIndex(creature.getNameID());
	ui->letterBox->setText(QString(creature.getLetter()));

	Color color = creature.getColor();
	ui->colorRedBox->setValue(color.red);
	ui->colorGreenBox->setValue(color.green);
	ui->colorBlueBox->setValue(color.blue);
	ui->raceBox->setCurrentIndex((unsigned char)(creature.getRace()));
	ui->genderBox->setCurrentIndex((unsigned char)(creature.getGender()));
	ui->alignmentBox->setCurrentIndex((unsigned char)(creature.getAlignment()));
	ui->stateBox->setCurrentIndex((unsigned char)(creature.getState()));

	ui->levelBox->setValue(creature.getLevel());
	ui->xpCurrentBox->setValue(creature.getXPCurrent());
	ui->xpNextLevelBox->setValue(creature.getXPCurrent() + creature.getXPRemaining());
	ui->hpCurrentBox->setValue(creature.getHPCurrent());
	ui->hpMaxBox->setValue(creature.getHPMax());
	ui->hpRegenerationBox->setValue(creature.getHPRegeneration());
	ui->mpCurrentBox->setValue(creature.getMPCurrent());
	ui->mpMaxBox->setValue(creature.getMPMax());
	ui->mpRegenerationBox->setValue(creature.getMPRegeneration());
	ui->damageBox->setValue(creature.getDamageMin());
	ui->damageDeltaBox->setValue(creature.getDamageMax() - creature.getDamageMin());
	ui->attackRateBox->setValue(creature.getAttackRate());
	ui->defenseBox->setValue(creature.getDefense());
	ui->defenseRateBox->setValue(creature.getDefenseRate());

	ui->strengthBox->setValue(creature.getAbilityValue(Ability::strength));
	ui->dexterityBox->setValue(creature.getAbilityValue(Ability::dexterity));
	ui->constitutionBox->setValue(creature.getAbilityValue(Ability::constitution));
	ui->intelligenceBox->setValue(creature.getAbilityValue(Ability::intelligence));
	ui->wisdomBox->setValue(creature.getAbilityValue(Ability::wisdom));

	size_t backpackSize = creature.inventory.getBackpackSize();
	ui->inventoryList->clear();
	for (size_t index = 0; index < backpackSize; ++index) {
		Item* item = creature.inventory.getBackpackItem(index);
		ui->inventoryList->addItem(QString::fromStdString(text[ {TextCategory::Item, item->getNameID()} ]));
		if (creature.isItemEquipped(item)) {
			QListWidgetItem* listItem = ui->inventoryList->item(index);
			QFont listItemFont = listItem->font();
			listItemFont.setWeight(QFont::Bold);
			listItem->setFont(listItemFont);
		}
	}
}

void CreatureEditor::updateCreatureParameters()
{
	creature.setNameID(ui->nameIDBox->currentIndex());

	std::string letterString = ui->letterBox->text().toStdString();
	if (!letterString.empty()) {
		creature.setLetter(letterString[0]);
	} else  {
		creature.setLetter('X');
	}

	Color color = {(uint8_t)(ui->colorRedBox->value()), (uint8_t)(ui->colorGreenBox->value()), (uint8_t)(ui->colorBlueBox->value())};
	creature.setColor(color);
	creature.setRace(Race(ui->raceBox->currentIndex()));
	creature.setGender(Gender(ui->genderBox->currentIndex()));
	creature.setAlignment(CreatureAlignment(ui->alignmentBox->currentIndex()));
	creature.setState(State(ui->stateBox->currentIndex()));

	creature.setLevel(ui->levelBox->value());
	creature.setXPCurrent(ui->xpCurrentBox->value());
	creature.setXPNextLevel(ui->xpNextLevelBox->value());
	creature.setHP(ui->hpCurrentBox->value());
	creature.setMP(ui->mpCurrentBox->value());

	creature.updateStats();
}

void CreatureEditor::on_inventoryList_itemDoubleClicked(QListWidgetItem*)
{
	unsigned int index = ui->inventoryList->currentIndex().row();
	if (!creature.equipItem(index)) {
		_LogInfo(text[String::Item(creature.inventory.getBackpackItem(index)->getNameID())] << " item requirements has not been satisfied");
	}

	updateEditorValues();
}
