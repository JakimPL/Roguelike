#include "area.hpp"
#include "functions.hpp"
#include "log.hpp"

Area::Area(GameObjects& gameObjects, const std::string& filename, bool fullPath) : areaObjects(gameObjects)
{
	loadFromFile(filename, fullPath);
}

Area::Area(GameObjects& gameObjects) : areaObjects(gameObjects)
{
	nameID = 0;
	width = 0;
	height = 0;
}

bool Area::loadFromFile(const std::string& filename, bool fullPath)
{
	bool success;
	std::string path = fullPath ? filename : Functions::getPath(filename, Filetype::ARE);
	_LogInfo("Opening " << path << " area file");
	std::ifstream resource(path, std::ios::in | std::ios::binary);
	if (resource.good()) {
		success = load(resource);
	} else {
		_LogError("Failed to open " << filename << " area file!");
		success = false;
	}

	resource.close();
	return success;
}

bool Area::saveToFile(const std::string& filename, bool fullPath)
{
	std::string path = fullPath ? filename : Functions::getPath(filename, Filetype::ARE);
	std::ofstream resource(path);
	if (resource.good()) {
		save(resource);

		_LogInfo("Saved " << path << " file succesfully");
		resource.close();
		return true;
	} else {
		_LogError("Failed to save " << path << " file!");
		return false;
	}
}

unsigned int Area::getNameID() const
{
	return nameID;
}

bool Area::load(std::ifstream& resource)
{
	char resourceHeader[SIZE_HEADER + 1];
	Functions::read(resource, resourceHeader, SIZE_HEADER);
	if (Functions::compareHeaders(headerARE, resourceHeader)) {
		// read map name, width and height
		resource.read((char*)&nameID, SIZE_INT);
		resource.read((char*)&width, SIZE_INT);
		resource.read((char*)&height, SIZE_INT);

		map.clear();
		map.resize(width);

		// read map characters
		for (unsigned int y = 0; y < height; ++y) {
			for (unsigned int x = 0; x < width; ++x) {
				char letter;
				Color color;
				bool obstacle;
				unsigned int nameID;

				resource.read(reinterpret_cast<char*>(&letter), SIZE_CHAR);
				resource.read(reinterpret_cast<char*>(&color), SIZE_COLOR);
				resource.read(reinterpret_cast<char*>(&obstacle), SIZE_CHAR);
				resource.read(reinterpret_cast<char*>(&nameID), SIZE_INT);

				Tile tile = {letter, color, obstacle, nameID};
				map[x].push_back(tile);
			}
		}

		_LogInfo("File opened successfully.");
		return true;
	} else {
		_LogError("Invalid area file!");
		return false;
	}
}

void Area::save(std::ofstream& resource)
{
	resource.write(headerARE, SIZE_HEADER);
	resource.write(reinterpret_cast<char*>(&nameID), SIZE_INT);
	resource.write(reinterpret_cast<char*>(&width), SIZE_INT);
	resource.write(reinterpret_cast<char*>(&height), SIZE_INT);

	// write characters
	for (unsigned int y = 0; y < height; ++y) {
		for (unsigned int x = 0; x < width; ++x) {
			Tile tile = getTile(x, y);
			resource.write(reinterpret_cast<char*>(&tile.letter), SIZE_CHAR);
			resource.write(reinterpret_cast<char*>(&tile.color), SIZE_COLOR);
			resource.write(reinterpret_cast<char*>(&tile.obstacle), SIZE_CHAR);
			resource.write(reinterpret_cast<char*>(&tile.nameID), SIZE_INT);
		}
	}

	for (GameObject* object : areaObjects) {

	}
}

unsigned int Area::getWidth() const
{
	return width;
}

unsigned int Area::getHeight() const
{
	return height;
}

Tile Area::getTile(unsigned int x, unsigned int y) const
{
	Tile tile;
	try {
		tile = map.at(x).at(y);
	}  catch (std::out_of_range &) {

	}

	return tile;
}

Tile Area::getTile(Position position) const
{
	return getTile(position.x, position.y);
}

void Area::setNameID(unsigned int value)
{
	nameID = value;
}

void Area::setDimensions(unsigned int w, unsigned int h)
{
	width = w;
	height = h;
	map.clear();
	map.resize(width);

	for (unsigned int y = 0; y < height; ++y) {
		for (unsigned int x = 0; x < width; ++x) {
			map[x].push_back(TILE_EMPTY);
		}
	}
}

void Area::setTile(unsigned int x, unsigned int y, Tile tile)
{
	map[x][y] = tile;
}

void Area::setTile(Position position, Tile tile)
{
	map[position.x][position.y] = tile;
}

bool Area::isPositionFree(int x, int y) const
{
	return isPositionFree({x, y});
}

bool Area::isPositionFree(Position position) const
{
	if (!isTileOutside(position) and getTile(position).obstacle) {
		return false;
	}

	for (GameObject* object : areaObjects) {
		if (object->isSolid() and object->getPosition() == position) {
			return false;
		}
	}

	return true;
}

GameObjects Area::isPositionTaken(int x, int y) const
{
	return isPositionTaken({x, y});
}

GameObjects Area::isPositionTaken(Position position) const
{
	GameObjects objectsAtPosition;
	for (GameObject* object : areaObjects) {
		if (object->getPosition() == position) {
			objectsAtPosition.push_back(object);
		}
	}

	return objectsAtPosition;
}

bool Area::isTileOutside(int x, int y) const
{
	return x < 0 or y < 0 or x >= int(width) or y >= int(height);
}

bool Area::isTileOutside(Position position) const
{
	return isTileOutside(position.x, position.y);
}
