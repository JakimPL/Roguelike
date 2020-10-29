#include "structures.hpp"

Area::Area(const std::string &filename)
{
	std::string path = PATH_ARE + filename + ARE_SUFFIX;
	_LogInfo("Opening " << path << " area file");
	std::ifstream resource(path, std::ios::in | std::ios::binary);
	if (resource.good()) {
		char resourceHeader[SIZE_HEADER];
		resource.read(resourceHeader, SIZE_HEADER);
		if (Functions::compareHeaders(headerARE, resourceHeader)) {
			// read map name, width and height
			resource.read((char*)&name, SIZE_INT);
			resource.read((char*)&width, SIZE_INT);
			resource.read((char*)&height, SIZE_INT);

			map.clear();
			std::vector<std::vector<char>> characterMap;
			std::vector<std::vector<Color>> colorMap;
			std::vector<std::vector<bool>> obstacleMap;

			// prepare map 2d vectors
			characterMap.resize(height);
			colorMap.resize(height);
			obstacleMap.resize(height);
			map.resize(height);

			///TODO: abstract version of block reading and error handling
			// read map characters
			for (unsigned int y = 0; y < height; ++y) {
				for (unsigned int x = 0; x < width; ++x) {
					char temp[SIZE_CHAR];
					resource.read(temp, SIZE_CHAR);
					characterMap[x].push_back(temp[0]);
				}
			}

			// read map colors
			for (unsigned int y = 0; y < height; ++y) {
				for (unsigned int x = 0; x < width; ++x) {
					char temp[SIZE_COLOR];
					resource.read(temp, SIZE_COLOR);
					Color color = {(unsigned int)(temp[0]), (unsigned int)(temp[1]), (unsigned int)(temp[2])};
					colorMap[x].push_back(color);
				}
			}

			// read map obstacle map
			for (unsigned int y = 0; y < height; ++y) {
				for (unsigned int x = 0; x < width; ++x) {
					char temp[SIZE_CHAR];
					resource.read(temp, SIZE_CHAR);
					bool obstacle = temp[0] > 0;
					obstacleMap[x].push_back(obstacle);
				}
			}

			// convert data to tile structure
			for (unsigned int y = 0; y < height; ++y) {
				for (unsigned int x = 0; x < width; ++x) {
					Tile tile = {characterMap[x][y], colorMap[x][y], obstacleMap[x][y]};
					map[x].push_back(tile);
				}
			}
		} else {
			_LogError("Invalid area file!");
		}
		resource.close();
	} else {
		_LogError("Failed to open " << filename << " file!");
	}

	_LogInfo(filename << ARE_SUFFIX << " opened successfully.");
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
	return map[x][y];
}

unsigned int Creature::getName()
{
	return name;
}

char Creature::getLetter()
{
	return letter;
}

Color Creature::getColor()
{
	return color;
}

Creature::Creature(const std::string &filename)
{

}

Creature::Creature()
{
	name = 0;
	color = {0, 0, 255};
	letter = 'X';
	race = Race::human;
	gender = Gender::male;
	state = 0;
	level = 1;
	xp_current = 0;
	xp_next_level = 100;
	xp_value = 0;
	gold = 50;
	hp = 30;
	hp_max = 30;
	mp = 15;
	mp_max = 15;
	ac_natural = 10;
	accuracy = 25;
	strength = 0;
	dexterity = 0;
	constitution = 0;
	intelligence = 0;
	wisdom = 0;
	effects = {};
	abilities = {};
}

bool Area::saveToFile(const std::string &filename)
{
	return false;
}

bool Character::saveToFile(const std::string &filename)
{
	return false;
}

bool Creature::saveToFile(const std::string &filename)
{
	return false;
}

bool Item::saveToFile(const std::string &filename)
{
	return false;
}
