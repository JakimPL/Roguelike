#include "dialog.hpp"
#include "functions.hpp"
#include "log.hpp"

Dialog::Dialog(const std::string& filename, bool fullPath)
{
	loadFromFile(filename, fullPath);
}

Dialog::Dialog()
{

}

bool Dialog::loadFromFile(const std::string& filename, bool fullPath)
{
	bool success;
	std::string path = fullPath ? filename : Functions::getPath(filename, Filetype::DLG);
	_LogInfo("Opening " << path << " dialog file");
	std::ifstream resource(path, std::ios::in);
	if (resource.good()) {
		success = load(resource);
	} else {
		_LogError("Failed to open " << filename << " dialog file!");
		success = false;
	}

	resource.close();
	return success;
}

bool Dialog::saveToFile(const std::string& filename, bool fullPath)
{
	std::string path = fullPath ? filename : Functions::getPath(filename, Filetype::DLG);
	std::ofstream resource(path);
	if (resource.good()) {
		save(resource);

		_LogInfo("Saved " << path << " dialog file succesfully");
		resource.close();
		return true;
	} else {
		_LogError("Failed to save " << path << " dialog file!");
		return false;
	}
}

bool Dialog::load(std::ifstream& resource)
{
	char resourceHeader[SIZE_HEADER + 1];
	Functions::read(resource, resourceHeader, SIZE_HEADER);
	if (Functions::compareHeaders(headerDLG, resourceHeader)) {
		resource.read(reinterpret_cast<char*>(&startDialogID), SIZE_INT);

		unsigned int size;
		resource.read(reinterpret_cast<char*>(&size), SIZE_INT);
		Dialogs dialogs;
		for (unsigned int index = 0; index < size; ++index) {
			unsigned int textID;
			resource.read(reinterpret_cast<char*>(&textID), SIZE_INT);

			unsigned int responsesSize;
			resource.read(reinterpret_cast<char*>(&responsesSize), SIZE_INT);
			Responses responses;
			for (unsigned int line = 0; line < responsesSize; ++line) {
				unsigned int textID;
				DialogCondition condition;
				DialogAction action;

				resource.read(reinterpret_cast<char*>(&textID), SIZE_INT);
				resource.read(reinterpret_cast<char*>(&condition), SIZE_DLGCOND);
				resource.read(reinterpret_cast<char*>(&action), SIZE_DLGACT);

				responses.push_back({textID, condition, action});
			}

			dialogs.push_back({textID, responses});
		}

		_LogInfo("Dialog file opened successfully.");
		return true;
	} else {
		_LogError("Invalid dialog file!");
		return false;
	}
}

void Dialog::save(std::ofstream& resource)
{
	resource.write(headerDLG, SIZE_HEADER);
	resource.write(reinterpret_cast<char*>(&startDialogID), SIZE_INT);

	unsigned int size = dialogs.size();
	resource.write(reinterpret_cast<char*>(&size), SIZE_INT);
	for (DialogLine line : dialogs) {
		resource.write(reinterpret_cast<char*>(&line.textID), SIZE_INT);

		unsigned int responsesSize = line.responses.size();
		resource.write(reinterpret_cast<char*>(&responsesSize), SIZE_INT);
		for (Response response : line.responses) {
			resource.write(reinterpret_cast<char*>(&response.textID), SIZE_INT);
			resource.write(reinterpret_cast<char*>(&response.condition), SIZE_DLGCOND);
			resource.write(reinterpret_cast<char*>(&response.action), SIZE_DLGACT);
		}
	}
}

DialogLine& Dialog::getLine(unsigned int index)
{
	return dialogs[index];
}

Response Dialog::getLineResponse(unsigned int index, unsigned responseID) const
{
	return dialogs[index].responses[responseID];
}

unsigned int Dialog::getLineTextID(unsigned int index) const
{
	return dialogs[index].textID;
}

unsigned int Dialog::getSize() const
{
	return dialogs.size();
}

void Dialog::setLineResponse(unsigned int index, unsigned responseID, Response response)
{
	dialogs[index].responses[responseID] = response;
}

void Dialog::setLineTextID(unsigned int index, unsigned int value)
{
	dialogs[index].textID = value;
}

void Dialog::addLine(DialogLine line)
{
	dialogs.push_back(line);
}

void Dialog::removeLine(unsigned int index)
{
	dialogs.erase(dialogs.begin() + index);
}

void DialogLine::addResponse(Response response)
{
	responses.push_back(response);
}

void DialogLine::removeResponse(unsigned int index)
{
	responses.erase(responses.begin() + index);
}
