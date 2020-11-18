#ifndef AREA_HPP
#define AREA_HPP

#include "structures.hpp"
#include "color.hpp"

struct Tile {
	char letter;
	Color color;
	bool obstacle;
	unsigned int nameID;
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

	unsigned int getNameID() const;
	unsigned int getHeight() const;
	unsigned int getWidth() const;
	Tile getTile(unsigned int x, unsigned int y) const;
	Tile getTile(Position position) const;

	bool isTileOutside(int x, int y) const;
	bool isTileOutside(Position position) const;
};

#endif // AREA_HPP
