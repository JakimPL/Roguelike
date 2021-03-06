#ifndef AREA_HPP
#define AREA_HPP

#include "structures.hpp"
#include "color.hpp"
#include "objects/gameobject.hpp"

struct Tile {
	char letter;
	Color color;
	bool obstacle;
	bool transparent;
	unsigned int nameID;

	bool operator==(Tile rhs) const
	{
		return letter == rhs.letter and
			   color == rhs.color and
			   obstacle == rhs.obstacle and
			   transparent == rhs.transparent and
			   nameID == rhs.nameID;
	};
};

struct Area {
private:
	GameObjects& areaObjects;

	unsigned int nameID;
	unsigned int width;
	unsigned int height;
	std::vector<std::vector<Tile>> map;

public:
	Area(GameObjects& gameObjects);
	Area(GameObjects& gameObjects, const std::string& filename, bool fullPath = false);

	unsigned int getNameID() const;
	unsigned int getHeight() const;
	unsigned int getWidth() const;
	Tile getTile(unsigned int x, unsigned int y) const;
	Tile getTile(Position position) const;

	void setNameID(unsigned int value);
	void setDimensions(unsigned int w, unsigned int h);
	void setTile(unsigned int x, unsigned int y, Tile tile);
	void setTile(Position position, Tile tile);

	bool isPositionFree(int x, int y, bool checkTransparency = false) const;
	bool isPositionFree(Position position, bool checkTransparency = false) const;
	GameObjects isPositionTaken(int x, int y) const;
	GameObjects isPositionTaken(Position position) const;
	bool isTileOutside(int x, int y) const;
	bool isTileOutside(Position position) const;

	bool loadFromFile(const std::string& filename, bool fullPath = false);
	bool saveToFile(const std::string& filename, bool fullPath = false);
	bool load(std::ifstream& resource);
	void save(std::ofstream& resource);
};

#endif // AREA_HPP
