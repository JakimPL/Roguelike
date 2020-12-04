#include "store.hpp"
#include "functions.hpp"
#include "log.hpp"

Store::Store(const std::vector<std::string> resourceList)
{
	resourceNames = resourceList;
}

Store::Store(const std::string& filename, bool fullPath)
{
	loadFromFile(filename, fullPath);
}

bool Store::loadFromFile(const std::string &filename, bool fullPath)
{
	bool success;
	std::string path = fullPath ? filename : Functions::getPath(filename, Filetype::STO);
	_LogInfo("Opening " << path << " store file");
	std::ifstream resource(path, std::ios::in | std::ios::binary);
	if (resource.good()) {
		success = load(resource);
	} else {
		_LogError("Failed to open " << filename << " store file!");
		success = false;
	}

	resource.close();
	return success;
}

bool Store::saveToFile(const std::string& filename, bool fullPath)
{
	std::string path = fullPath ? filename : Functions::getPath(filename, Filetype::STO);
	std::ofstream resource(path);
	if (resource.good()) {
		save(resource);
		_LogInfo("Saved " << path << " store file succesfully");
		resource.close();
		return true;
	} else {
		_LogError("Failed to save " << path << " store file!");
		return false;
	}
}

bool Store::load(std::ifstream& resource)
{
	char resourceHeader[SIZE_HEADER + 1];
	Functions::read(resource, resourceHeader, SIZE_HEADER);
	if (Functions::compareHeaders(headerSTO, resourceHeader)) {
		unsigned int size;
		resource.read(reinterpret_cast<char*>(&size), SIZE_INT);
		resourceNames.clear();
		for (size_t index = 0; index < size; ++index) {
			unsigned int itemSize;
			resource.read(reinterpret_cast<char*>(&itemSize), SIZE_INT);
			resourceNames.push_back(Functions::readString(resource, itemSize));
		}

		// effects to be implemented
		_LogInfo("File opened successfully.");
		return true;
	} else {
		_LogError("Invalid store file!");
		return false;
	}
}

void Store::save(std::ofstream& resource)
{
	resource.write(headerSTO, SIZE_HEADER);
	unsigned int size = resourceNames.size();
	resource.write(reinterpret_cast<char*>(&size), SIZE_INT);
	for (size_t index = 0; index < size; ++index) {
		unsigned int itemSize = resourceNames[index].size();
		resource.write(reinterpret_cast<char*>(&itemSize), SIZE_INT);
		resource.write(resourceNames[index].c_str(), itemSize);
	}
}
