#include "structures.hpp"

void Position::moveInDirection(int length, Direction chosenDirection)
{
	int dir = (chosenDirection == COUNT ? Direction(direction) : chosenDirection);
	x += length * DIR_X(dir);
	y += length * DIR_Y(dir);
}

const Position Position::operator +(int length) const
{
	Position newPosition(*this);
	newPosition.moveInDirection(length);
	return newPosition;
}

Area::Area(const std::string& filename)
{
	std::string path = Functions::getPath(filename, ARE);
	_LogInfo("Opening " << path << " area file");
	std::ifstream resource(path, std::ios::in | std::ios::binary);
	if (resource.good()) {
		char resourceHeader[SIZE_HEADER + 1];
		Functions::read(resource, resourceHeader, SIZE_HEADER);
		if (Functions::compareHeaders(headerARE, resourceHeader)) {
			// read map name, width and height
			resource.read((char*)&textID, SIZE_INT);
			resource.read((char*)&width, SIZE_INT);
			resource.read((char*)&height, SIZE_INT);

			map.clear();
			std::vector<std::vector<char>> characterMap;
			std::vector<std::vector<Color>> colorMap;
			std::vector<std::vector<bool>> obstacleMap;
			std::vector<std::vector<unsigned int>> textIDMap;

			// prepare map 2d vectors
			characterMap.resize(height);
			colorMap.resize(height);
			obstacleMap.resize(height);
			textIDMap.resize(height);
			map.resize(height);

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

			// read tile textID map
			for (unsigned int y = 0; y < height; ++y) {
				for (unsigned int x = 0; x < width; ++x) {
					unsigned int textID;
					resource.read(reinterpret_cast<char*>(&textID), SIZE_INT);
					textIDMap[x].push_back(textID);
				}
			}

			// convert data to tile structure
			for (unsigned int y = 0; y < height; ++y) {
				for (unsigned int x = 0; x < width; ++x) {
					Tile tile = {characterMap[x][y], colorMap[x][y], obstacleMap[x][y], textIDMap[x][y]};
					map[x].push_back(tile);
				}
			}
		} else {
			_LogError("Invalid area file!");
		}
		resource.close();
	} else {
		_LogError("Failed to open " << filename << " area file!");
	}

	_LogInfo("File " << path << " opened successfully.");
}

unsigned int Area::getWidth()
{
	return width;
}

unsigned int Area::getHeight()
{
	return height;
}

Tile Area::getTile(unsigned int x, unsigned int y)
{
	Tile tile;
	try {
		tile = map.at(x).at(y);
	}  catch (std::out_of_range &) {

	}

	return tile;
}

Tile Area::getTile(Position position)
{
	return getTile(position.x, position.y);
}

unsigned int Area::getTextID()
{
	return textID;
}

bool Area::saveToFile(const std::string&)
{
	return false;
}

bool Character::saveToFile(const std::string&)
{
	return false;
}
