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

Area::Area(const std::string filename)
{
	std::string path = PATH_ARE + filename + ARE_SUFFIX;
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

unsigned int Area::getTextID()
{
	return textID;
}

Creature::Creature(const std::string)
{

}

Creature::Creature()
{
	textID = 0;
	color = {255, 255, 0};
	letter = 'X';
	race = Race::human;
	gender = Gender::male;
	state = 0;
	level = 1;
	xp = 0;
	xpNextLevel = 100;
	xpValue = 0;
	gold = 50;
	hp = 30;
	hpMax = 30;
	mp = 15;
	mpMax = 15;
	acBase = 10;
	accuracy = 25;
	strength = 0;
	dexterity = 0;
	constitution = 0;
	intelligence = 0;
	wisdom = 0;
	effects = {};
	abilities = {};
}

unsigned int Creature::getTextID()
{
	return textID;
}

char Creature::getLetter()
{
	return letter;
}

Color Creature::getColor()
{
	return color;
}

int Creature::getLevel()
{
	return level;
}

int Creature::getRemainingXP()
{
	return xpNextLevel - xp;
}

int Creature::getCurrentXP()
{
	return xp;
}

int Creature::getCurrentHP()
{
	return hp;
}

int Creature::getCurrentMP()
{
	return mp;
}

int Creature::getMaxHP()
{
	return hpMax;
}

int Creature::getMaxMP()
{
	return mpMax;
}

unsigned int Creature::getWeaponTextID()
{
	return (unsigned int)(String::Fist);
}

bool Area::saveToFile(const std::string)
{
	return false;
}

bool Character::saveToFile(const std::string)
{
	return false;
}

bool Creature::saveToFile(const std::string)
{
	return false;
}

bool Item::saveToFile(const std::string)
{
	return false;
}
