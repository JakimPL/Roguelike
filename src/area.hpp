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

	void setNameID(unsigned int value);
	void setHeight(unsigned int value);
	void setWidth(unsigned int value);
	void setTile(unsigned int x, unsigned int y, Tile tile);
	void setTile(Position position, Tile tile);

	bool isTileOutside(int x, int y) const;
	bool isTileOutside(Position position) const;
};

#endif // AREA_HPP
