#ifndef AREA_HPP
#define AREA_HPP

#include "structures.hpp"
#include "color.hpp"

struct Tile {
	char letter = ' ';
	Color color = {0, 0, 0};
	bool obstacle = 0;
	unsigned int nameID = 0;
};

struct Area {
private:
	unsigned int nameID;
	unsigned int width;
	unsigned int height;
	std::vector<std::vector<Tile>> map;

public:
	Area();
	Area(const std::string& filename, bool fullPath = false);
	bool saveToFile(const std::string& filename, bool fullPath = false);
	unsigned int getNameID();
	unsigned int getHeight();
	unsigned int getWidth();
	Tile getTile(unsigned int x, unsigned int y);
	Tile getTile(Position position);
};

#endif // AREA_HPP
