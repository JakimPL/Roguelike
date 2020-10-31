#include "item.hpp"

Item::Item(const std::string& filename)
{
	std::string path = Functions::getPath(filename, ITM);
	_LogInfo("Opening " << path << " area file");
	std::ifstream resource(path, std::ios::in | std::ios::binary);
	if (resource.good()) {
		char resourceHeader[SIZE_HEADER + 1];
		Functions::read(resource, resourceHeader, SIZE_HEADER);
		if (Functions::compareHeaders(headerITM, resourceHeader)) {
			resource.read(reinterpret_cast<char*>(&textID), SIZE_INT);
			resource.read(reinterpret_cast<char*>(&descriptionID), SIZE_INT);
			resource.read(reinterpret_cast<char*>(&color), SIZE_COLOR);
			resource.read(reinterpret_cast<char*>(&type), SIZE_CHAR);
			resource.read(reinterpret_cast<char*>(&category), SIZE_CHAR);
			resource.read(reinterpret_cast<char*>(&flag), SIZE_CHAR);
			resource.read(reinterpret_cast<char*>(&price), SIZE_LONG);
			resource.read(reinterpret_cast<char*>(&attack), SIZE_INT);
			resource.read(reinterpret_cast<char*>(&attackRate), SIZE_INT);
			resource.read(reinterpret_cast<char*>(&damage), SIZE_INT);
			resource.read(reinterpret_cast<char*>(&damageDelta), SIZE_INT);
			resource.read(reinterpret_cast<char*>(&delay), SIZE_INT);
			resource.read(reinterpret_cast<char*>(&defense), SIZE_INT);
			resource.read(reinterpret_cast<char*>(&defenseRate), SIZE_INT);
			resource.read(reinterpret_cast<char*>(&requiredLevel), SIZE_INT);
			resource.read(reinterpret_cast<char*>(&requiredStrength), SIZE_INT);
			resource.read(reinterpret_cast<char*>(&requiredWisdom), SIZE_INT);
			resource.read(reinterpret_cast<char*>(&requiredDexterity), SIZE_INT);
			resource.read(reinterpret_cast<char*>(&requiredIntelligence), SIZE_INT);

			// effects to be implemented
		} else {
			_LogError("Invalid item file!");
		}
		resource.close();
	} else {
		_LogError("Failed to open " << filename << " item file!");
	}

	_LogInfo("File " << path << " opened successfully.");
}

unsigned int Item::getTextID()
{
	return textID;
}

Color Item::getColor()
{
	return color;
}

bool Item::saveToFile(const std::string& filename)
{
	std::string path = Functions::getPath(filename, ITM);
	std::ofstream resource(path);
	if (resource.good()) {
		resource.write(headerITM, SIZE_HEADER);
		resource.write(reinterpret_cast<char*>(&textID), SIZE_INT);
		resource.write(reinterpret_cast<char*>(&descriptionID), SIZE_INT);
		resource.write(reinterpret_cast<char*>(&color), SIZE_COLOR);
		resource.write(reinterpret_cast<char*>(&type), SIZE_CHAR);
		resource.write(reinterpret_cast<char*>(&category), SIZE_CHAR);
		resource.write(reinterpret_cast<char*>(&flag), SIZE_CHAR);
		resource.write(reinterpret_cast<char*>(&price), SIZE_LONG);
		resource.write(reinterpret_cast<char*>(&attack), SIZE_INT);
		resource.write(reinterpret_cast<char*>(&attackRate), SIZE_INT);
		resource.write(reinterpret_cast<char*>(&damage), SIZE_INT);
		resource.write(reinterpret_cast<char*>(&damageDelta), SIZE_INT);
		resource.write(reinterpret_cast<char*>(&delay), SIZE_INT);
		resource.write(reinterpret_cast<char*>(&defense), SIZE_INT);
		resource.write(reinterpret_cast<char*>(&defenseRate), SIZE_INT);
		resource.write(reinterpret_cast<char*>(&requiredLevel), SIZE_INT);
		resource.write(reinterpret_cast<char*>(&requiredStrength), SIZE_INT);
		resource.write(reinterpret_cast<char*>(&requiredWisdom), SIZE_INT);
		resource.write(reinterpret_cast<char*>(&requiredDexterity), SIZE_INT);
		resource.write(reinterpret_cast<char*>(&requiredIntelligence), SIZE_INT);

		unsigned int size = effects.size();
		resource.write(reinterpret_cast<char*>(&size), SIZE_INT);
		for (size_t it = 0; it < size; ++it) {
			// to be implemented
		}

		_LogInfo("Saved " << path << " file succesfully");
		resource.close();
		return true;
	} else {
		_LogError("Failed to save " << path << " file!");
		return false;
	}
}

bool Inventory::addItem(const std::string& filename)
{
	if (!isFull()) {
		backpack.push_back(filename);
		return true;
	}

	return false;
}

bool Inventory::addItem(Item item)
{
	if (!isFull()) {
		backpack.push_back(item);
		return true;
	}

	return false;
}

Item* Inventory::getBackpackItem(unsigned int index)
{
	if (index >= backpack.size()) {
		return nullptr;
	} else {
		return &backpack[index];
	}
}

unsigned int Inventory::getBackpackSize()
{
	return backpack.size();
}

bool Inventory::isFull() const
{
	return backpack.size() >= INVENTORY_SIZE;
}
