#include "area.hpp"
#include "functions.hpp"
#include "log.hpp"

Area::Area(const std::string& filename, bool fullPath)
{
	std::string path = fullPath ? filename : Functions::getPath(filename, ARE);
	_LogInfo("Opening " << path << " area file");
	std::ifstream resource(path, std::ios::in | std::ios::binary);
	if (resource.good()) {
		char resourceHeader[SIZE_HEADER + 1];
		Functions::read(resource, resourceHeader, SIZE_HEADER);
		if (Functions::compareHeaders(headerARE, resourceHeader)) {
			// read map name, width and height
			resource.read((char*)&nameID, SIZE_INT);
			resource.read((char*)&width, SIZE_INT);
			resource.read((char*)&height, SIZE_INT);

			map.clear();
			std::vector<std::vector<char>> characterMap;
			std::vector<std::vector<Color>> colorMap;
			std::vector<std::vector<bool>> obstacleMap;
			std::vector<std::vector<unsigned int>> nameIDMap;

			// prepare map 2d vectors
			characterMap.resize(width);
			colorMap.resize(width);
			obstacleMap.resize(width);
			nameIDMap.resize(width);
			map.resize(width);

			///TODO: abstract version of block reading and error handling
			// read map characters
			for (unsigned int y = 0; y < height; ++y) {
				for (unsigned int x = 0; x < width; ++x) {
					char character;
					resource.read(reinterpret_cast<char*>(&character), SIZE_CHAR);
					characterMap[x].push_back(character);
				}
			}

			// read map colors
			for (unsigned int y = 0; y < height; ++y) {
				for (unsigned int x = 0; x < width; ++x) {
					Color color;
					resource.read(reinterpret_cast<char*>(&color), SIZE_COLOR);
					colorMap[x].push_back(color);
				}
			}

			// read obstacle map
			for (unsigned int y = 0; y < height; ++y) {
				for (unsigned int x = 0; x < width; ++x) {
					bool obstacle;
					resource.read(reinterpret_cast<char*>(&obstacle), SIZE_CHAR);
					obstacleMap[x].push_back(obstacle);
				}
			}

			// read tile nameID map
			for (unsigned int y = 0; y < height; ++y) {
				for (unsigned int x = 0; x < width; ++x) {
					unsigned int nameID;
					resource.read(reinterpret_cast<char*>(&nameID), SIZE_INT);
					nameIDMap[x].push_back(nameID);
				}
			}

			// convert data to tile structure
			for (unsigned int y = 0; y < height; ++y) {
				for (unsigned int x = 0; x < width; ++x) {
					Tile tile = {characterMap[x][y], colorMap[x][y], obstacleMap[x][y], nameIDMap[x][y]};
					map[x].push_back(tile);
				}
			}

			_LogInfo("File " << path << " opened successfully.");
		} else {
			_LogError("Invalid area file!");
		}
	} else {
		_LogError("Failed to open " << filename << " area file!");
	}

	resource.close();
}

Area::Area()
{
	nameID = 0;
	width = 0;
	height = 0;
}

bool Area::saveToFile(const std::string&, bool)
{
	return false;
}

unsigned int Area::getNameID() const
{
	return nameID;
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

void Area::setHeight(unsigned int value)
{
	height = value;
}

void Area::setWidth(unsigned int value)
{
	width = value;
}

void Area::setTile(unsigned int x, unsigned int y, Tile tile)
{
	map[x][y] = tile;
}

void Area::setTile(Position position, Tile tile)
{
	map[position.x][position.y] = tile;
}

bool Area::isTileOutside(int x, int y) const
{
	return x < 0 or y < 0 or x >= int(width) or y >= int(height);
}

bool Area::isTileOutside(Position position) const
{
	return isTileOutside(position.x, position.y);
}

