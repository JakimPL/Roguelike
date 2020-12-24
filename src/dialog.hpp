#ifndef DIALOG_HPP
#define DIALOG_HPP

#include "globalvariables.hpp"

#include <vector>
#include <fstream>
#include <sstream>

enum class ComparisonOperator {
	isEqual,
	isDifferent,
	isLessOrEqualThan,
	isLessThan,
	isGreaterOrEqualThan,
	isGreaterThan,
	count
};

static const std::vector<std::string> OPERATOR_STRING = {
	"=",
	"≠",
	"≤",
	"<",
	"≥",
	">"
};

enum ResponseAction {
	ra_OPEN_STORE = -2,
	ra_EXIT
};

struct DialogStart {
	bool useGlobalVariable = false;
	int dialogID = 0;
};

struct DialogCondition {
	GlobalVariable variable = g_NONE;
	ComparisonOperator comparisonOperator = ComparisonOperator::isEqual;
	bool compareWithVariable = false;
	int value = 0;
};

struct DialogAction {
	DialogStart nextDialog;
	GlobalVariable variable = g_NONE;
	int value = 0;
};

struct DialogResponse {
	unsigned int textID;
	DialogCondition condition;
	DialogAction action;
};

typedef std::vector<DialogResponse> Responses;

struct DialogLine {
	unsigned int textID = 0;
	std::vector<unsigned int> responsesID;

	void addResponseID(unsigned int id);
	void removeResponseID(unsigned int index);

	unsigned int getResponsesCount() const;
	unsigned int getResponseID(unsigned int index) const;
	void setResponseID(unsigned int index, unsigned int id);
};

typedef std::vector<DialogLine> Dialogs;

constexpr int SIZE_DLGSTART = sizeof(DialogStart);
constexpr int SIZE_DLGCOND = sizeof(DialogCondition);
constexpr int SIZE_DLGACT = sizeof(DialogAction);
constexpr int SIZE_DLGRESP = sizeof(DialogResponse);

class Dialog
{
private:
	DialogStart startDialog;
	Dialogs dialogs;
	Responses responses;

public:
	Dialog();
	Dialog(const std::string& filename, bool fullPath = false);

	DialogLine& getLine(unsigned int index);
	DialogResponse& getResponse(unsigned int index);
	DialogResponse& getResponseByID(unsigned int index, unsigned int responseID);
	int getStartDialogID(GlobalState* state) const;
	int getStartDialogIDValue() const;
	bool getUseGlobalVariable() const;

	unsigned int getLinesCount() const;
	unsigned int getResponsesCount() const;

	void addLine(DialogLine line);
	void addResponse(DialogResponse response);
	void removeResponse(unsigned int index);
	void removeLine(unsigned int index);
	void setStartDialogIDValue(unsigned int index);
	void setUseGlobalVariable(bool value);

	bool loadFromFile(const std::string& filename, bool fullPath = false);
	bool saveToFile(const std::string& filename, bool fullPath = false);
	bool load(std::ifstream& resource);
	void save(std::ofstream& resource);
};

#endif // DIALOG_HPP
