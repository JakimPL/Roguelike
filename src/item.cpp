#include "item.hpp"
#include "functions.hpp"
#include "log.hpp"
#include "options.hpp"

Item::Item(const std::string& filename, bool fullPath)
{
	loadFromFile(filename, fullPath);
}

Item::Item()
{
	nameID = 0;
	descriptionID = 0;
	color = {255, 255, 255};
	type = ItemType::miscellaneous;
	category = ItemCategory::unused;
	flag = ItemFlag::none;
	price = 0;
	damage = 0;
	damageDelta = 0;
	attackRate = 0;
	delay = 0;
	defense = 0;
	defenseRate = 0;
	requiredLevel = 0;

	for (size_t abilityIndex = 0; abilityIndex < Ability::count; ++abilityIndex) {
		Ability ability = Ability(abilityIndex);
		requiredAbilities[ability] = 0;
	}
}

bool Item::loadFromFile(const std::string &filename, bool fullPath)
{
	bool success;
	std::string path = fullPath ? filename : Functions::getPath(filename, Filetype::ITM);
	_LogInfo("Opening " << path << " item file");
	std::ifstream resource(path, std::ios::in | std::ios::binary);
	if (resource.good()) {
		success = load(resource);
	} else {
		_LogError("Failed to open " << filename << " item file!");
		success = false;
	}

	resource.close();
	return success;
}

bool Item::saveToFile(const std::string& filename, bool fullPath)
{
	std::string path = fullPath ? filename : Functions::getPath(filename, Filetype::ITM);
	std::ofstream resource(path);
	if (resource.good()) {
		save(resource);
		_LogInfo("Saved " << path << " item file succesfully");
		resource.close();
		return true;
	} else {
		_LogError("Failed to save " << path << " item file!");
		return false;
	}
}

bool Item::load(std::ifstream& resource)
{
	char resourceHeader[SIZE_HEADER + 1];
	Functions::read(resource, resourceHeader, SIZE_HEADER);
	if (Functions::compareHeaders(headerITM, resourceHeader)) {
		resource.read(reinterpret_cast<char*>(&nameID), SIZE_INT);
		resource.read(reinterpret_cast<char*>(&descriptionID), SIZE_INT);
		resource.read(reinterpret_cast<char*>(&color), SIZE_COLOR);
		resource.read(reinterpret_cast<char*>(&type), SIZE_CHAR);
		resource.read(reinterpret_cast<char*>(&category), SIZE_CHAR);
		resource.read(reinterpret_cast<char*>(&flag), SIZE_CHAR);
		resource.read(reinterpret_cast<char*>(&price), SIZE_LONG);
		resource.read(reinterpret_cast<char*>(&damage), SIZE_INT);
		resource.read(reinterpret_cast<char*>(&damageDelta), SIZE_INT);
		resource.read(reinterpret_cast<char*>(&attackRate), SIZE_INT);
		resource.read(reinterpret_cast<char*>(&delay), SIZE_INT);
		resource.read(reinterpret_cast<char*>(&defense), SIZE_INT);
		resource.read(reinterpret_cast<char*>(&defenseRate), SIZE_INT);
		resource.read(reinterpret_cast<char*>(&requiredLevel), SIZE_INT);
		for (size_t ability = 0; ability < Ability::count; ++ability) {
			resource.read(reinterpret_cast<char*>(&requiredAbilities[ability]), SIZE_INT);
		}

		// effects to be implemented
		_LogInfo("Item file opened successfully.");
		return true;
	} else {
		_LogError("Invalid item file!");
		return false;
	}
}

void Item::save(std::ofstream& resource)
{
	resource.write(headerITM, SIZE_HEADER);
	resource.write(reinterpret_cast<char*>(&nameID), SIZE_INT);
	resource.write(reinterpret_cast<char*>(&descriptionID), SIZE_INT);
	resource.write(reinterpret_cast<char*>(&color), SIZE_COLOR);
	resource.write(reinterpret_cast<char*>(&type), SIZE_CHAR);
	resource.write(reinterpret_cast<char*>(&category), SIZE_CHAR);
	resource.write(reinterpret_cast<char*>(&flag), SIZE_CHAR);
	resource.write(reinterpret_cast<char*>(&price), SIZE_LONG);
	resource.write(reinterpret_cast<char*>(&damage), SIZE_INT);
	resource.write(reinterpret_cast<char*>(&damageDelta), SIZE_INT);
	resource.write(reinterpret_cast<char*>(&attackRate), SIZE_INT);
	resource.write(reinterpret_cast<char*>(&delay), SIZE_INT);
	resource.write(reinterpret_cast<char*>(&defense), SIZE_INT);
	resource.write(reinterpret_cast<char*>(&defenseRate), SIZE_INT);
	resource.write(reinterpret_cast<char*>(&requiredLevel), SIZE_INT);
	for (size_t ability = 0; ability < Ability::count; ++ability) {
		resource.write(reinterpret_cast<char*>(&requiredAbilities[ability]), SIZE_INT);
	}

	unsigned int size = effects.size();
	resource.write(reinterpret_cast<char*>(&size), SIZE_INT);
	for (size_t it = 0; it < size; ++it) {
		// to be implemented
	}
}

unsigned int Item::getNameID() const
{
	return nameID;
}

unsigned int Item::getDescriptionID() const
{
	return descriptionID;
}

Color Item::getColor() const
{
	return color;
}

ItemType Item::getType() const
{
	return type;
}

ItemCategory Item::getCategory() const
{
	return category;
}

ItemFlag Item::getFlag() const
{
	return flag;
}

unsigned long Item::getPrice() const
{
	return price;
}

int Item::getDamage() const
{
	return damage;
}

int Item::getDamageDelta() const
{
	return damageDelta;
}

int Item::getAttackRate() const
{
	return attackRate;
}

int Item::getDelay() const
{
	return delay;
}

int Item::getDefense() const
{
	return defense;
}

int Item::getDefenseRate() const
{
	return defenseRate;
}

int Item::getRequiredLevel() const
{
	return requiredLevel;
}

int Item::getRequiredAbility(const Ability ability) const
{
	return requiredAbilities[ability];
}

void Item::setNameID(unsigned int value)
{
	nameID = value;
}

void Item::setDescriptionID(unsigned int value)
{
	descriptionID = value;
}

void Item::setColor(Color value)
{
	color = value;;
}

void Item::setType(ItemType value)
{
	type = value;
}

void Item::setCategory(ItemCategory value)
{
	category = value;
}

void Item::setFlag(ItemFlag value)
{
	flag = value;
}

void Item::setPrice(unsigned long value)
{
	price = value;
}

void Item::setDamage(int value)
{
	damage = value;
}

void Item::setDamageDelta(int value)
{
	damageDelta = value;
}

void Item::setAttackRate(int value)
{
	attackRate = value;
}

void Item::setDelay(unsigned int value)
{
	delay = value;
}

void Item::setDefense(int value)
{
	defense = value;
}

void Item::setDefenseRate(int value)
{
	defenseRate = value;
}

void Item::setRequiredLevel(unsigned int value)
{
	requiredLevel = value;
}

void Item::setRequiredAbility(const Ability ability, unsigned int value)
{
	requiredAbilities[ability] = value;
}

Inventory::Inventory()
{
	for (unsigned int type = 0; type < (unsigned int)(ItemType::count); ++type) {
		stack[ItemType(type)] = -1;
	}
}

unsigned int Inventory::addItem(const std::string& filename)
{
	if (!isFull()) {
		backpack.push_back(filename);
		return getBackpackSize() - 1;
	}

	return -1;
}

unsigned int Inventory::addItem(Item item)
{
	if (!isFull()) {
		backpack.push_back(item);
		return getBackpackSize() - 1;
	}

	return -1;
}

void Inventory::clear()
{
	stack.clear();
	while (!backpack.empty()) {
		backpack.pop_back();
	}
}

void Inventory::dropItem(ItemType type)
{
	stack[type] = -1;
}

void Inventory::dropItem(Item* item)
{
	if (item == nullptr) {
		_LogError("Trying to drop NULL item!");
		return;
	}

	stack[item->getType()] = -1;
}

void Inventory::equipItem(int index)
{
	Item* item = getBackpackItem(index);
	stack[item->getType()] = index;
}

void Inventory::removeItem(int index)
{
	if (size_t(index) < getBackpackSize()) {
		ItemType type = backpack[index].getType();
		if (index == stack[type]) {
			dropItem(type);
		}

		for (unsigned int itemType = 0; itemType < (unsigned int)(ItemType::count); ++itemType) {
			if (stack[ItemType(itemType) ] > index) {
				stack[ItemType(itemType) ]--;
			}
		}

		backpack.erase(backpack.begin() + index);
	} else {
		_LogError("Bad inventory index!");
	}
}

Item* Inventory::getBackpackItem(int index)
{
	if (size_t(index) >= backpack.size()) {
		return nullptr;
	} else {
		return &backpack[index];
	}
}

int Inventory::getStackItemIndex(ItemType type)
{
	return stack[type];
}

Item* Inventory::getStackItem(ItemType type)
{
	return getBackpackItem(getStackItemIndex(type));
}

unsigned int Inventory::getBackpackSize()
{
	return backpack.size();
}

bool Inventory::isEmpty() const
{
	return backpack.size() == 0;
}

bool Inventory::isFull() const
{
	return backpack.size() >= INVENTORY_SIZE;
}
