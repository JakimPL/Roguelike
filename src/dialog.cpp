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
		dialogs.clear();
		responses.clear();

		resource.read(reinterpret_cast<char*>(&startDialog.useGlobalVariable), SIZE_CHAR);
		resource.read(reinterpret_cast<char*>(&startDialog.dialogID), SIZE_INT);

		unsigned int size;
		resource.read(reinterpret_cast<char*>(&size), SIZE_INT);
		for (unsigned int index = 0; index < size; ++index) {
			unsigned int textID;
			resource.read(reinterpret_cast<char*>(&textID), SIZE_INT);

			unsigned int responsesSize;
			std::vector<unsigned int> responsesID;
			resource.read(reinterpret_cast<char*>(&responsesSize), SIZE_INT);
			for (unsigned int index = 0; index < responsesSize; ++index) {
				unsigned int responseID;
				resource.read(reinterpret_cast<char*>(&responseID), SIZE_INT);
				responsesID.push_back(responseID);
			}

			dialogs.push_back({textID, responsesID});
		}

		resource.read(reinterpret_cast<char*>(&size), SIZE_INT);
		for (unsigned int index = 0; index < size; ++index) {
			DialogResponse response;
			resource.read(reinterpret_cast<char*>(&response), SIZE_DLGRESP);
			responses.push_back(response);
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
	resource.write(reinterpret_cast<char*>(&startDialog.useGlobalVariable), SIZE_CHAR);
	resource.write(reinterpret_cast<char*>(&startDialog.dialogID), SIZE_INT);

	unsigned int dialogsSize = dialogs.size();
	resource.write(reinterpret_cast<char*>(&dialogsSize), SIZE_INT);
	for (DialogLine line : dialogs) {
		unsigned int lineSize = line.responsesID.size();
		resource.write(reinterpret_cast<char*>(&line.textID), SIZE_INT);
		resource.write(reinterpret_cast<char*>(&lineSize), SIZE_INT);
		for (unsigned int index = 0; index < lineSize; ++index) {
			resource.write(reinterpret_cast<char*>(&line.responsesID[index]), SIZE_INT);
		}
	}

	unsigned int responsesSize = responses.size();
	resource.write(reinterpret_cast<char*>(&responsesSize), SIZE_INT);
	for (DialogResponse response : responses) {
		resource.write(reinterpret_cast<char*>(&response), SIZE_DLGRESP);
	}
}

DialogLine& Dialog::getLine(unsigned int index)
{
	try {
		return dialogs.at(index);
	} catch (...) {
		_LogError("Failed to load a dialog of id: " << index);
		throw std::runtime_error("failed to load a dialog");
	}
}

DialogResponse& Dialog::getResponse(unsigned int index)
{
	try {
		return responses.at(index);
	} catch (...) {
		_LogError("Failed to load a response of id: " << index);
		throw std::runtime_error("failed to load a response");
	}
}

DialogResponse& Dialog::getResponseByID(unsigned int index, unsigned int responseID)
{
	return getResponse(getLine(index).getResponseID(responseID));
}

int Dialog::getStartDialogID(GlobalState* state) const
{
	return startDialog.useGlobalVariable ? state->getVariable(GlobalVariable(startDialog.dialogID)) : startDialog.dialogID;
}

int Dialog::getStartDialogIDValue() const
{
	return startDialog.dialogID;
}

bool Dialog::getUseGlobalVariable() const
{
	return startDialog.useGlobalVariable;
}

unsigned int Dialog::getLinesCount() const
{
	return dialogs.size();
}

unsigned int Dialog::getResponsesCount() const
{
	return responses.size();
}

void Dialog::setStartDialogIDValue(unsigned int index)
{
	startDialog.dialogID = index;
}

void Dialog::setUseGlobalVariable(bool value)
{
	startDialog.useGlobalVariable = value;
}

void Dialog::addLine(DialogLine line)
{
	dialogs.push_back(line);
}

void Dialog::removeLine(unsigned int index)
{
	dialogs.erase(dialogs.begin() + index);
}

void Dialog::addResponse(DialogResponse response)
{
	responses.push_back(response);
}

void Dialog::removeResponse(unsigned int index)
{
	responses.erase(responses.begin() + index);
}

void DialogLine::addResponseID(unsigned int id)
{
	responsesID.push_back(id);
}

void DialogLine::removeResponseID(unsigned int index)
{
	responsesID.erase(responsesID.begin() + index);
}

unsigned int DialogLine::getResponseID(unsigned int index) const
{
	try {
		return responsesID.at(index);
	} catch (std::out_of_range&) {
		_LogError("Failed to get a response id of an index: " << index);
		throw std::runtime_error("failed to get a response ID");
	}
}

unsigned int DialogLine::getResponsesCount() const
{
	return responsesID.size();
}

void DialogLine::setResponseID(unsigned int index, unsigned int id)
{
	try {
		responsesID.at(index) = id;
	} catch (std::out_of_range&) {
		_LogError("Failed to set a response id of an index: " << index);
		throw std::runtime_error("failed to set a response ID");
	}
}
