#ifndef STORE_HPP
#define STORE_HPP

#include "item.hpp"

struct Store {
	std::vector<std::string> resourceNames;
	Inventory inventory;

public:
	Store();
	Store(const std::vector<std::string> resourceList);
	Store(const std::string& filename, bool fullPath = false);

	bool loadFromFile(const std::string& filename, bool fullPath = false);
	bool saveToFile(const std::string& filename, bool fullPath = false);
	bool load(std::ifstream& resource);
	void save(std::ofstream& resource);
};

#endif // STORE_HPP
