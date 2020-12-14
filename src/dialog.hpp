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

struct DialogCondition {
	GlobalVariable variable = g_NONE;
	ComparisonOperator comparisonOperator = ComparisonOperator::isEqual;
	bool compareWithVariable = false;
	int value = 0;
};

struct DialogAction {
	unsigned int nextDialogID = -1;
	GlobalVariable variable = g_NONE;
	int value = 0;
};

struct Response {
	unsigned int textID = 0;
	DialogCondition condition;
	DialogAction action;
};

typedef std::vector<Response> Responses;

struct DialogLine {
	unsigned int textID = 0;
	Responses responses;

	void addResponse(Response response);
	void removeResponse(unsigned int index);
};

typedef std::vector<DialogLine> Dialogs;

constexpr int SIZE_DLGCOND = sizeof(DialogCondition);
constexpr int SIZE_DLGACT = sizeof(DialogAction);

class Dialog
{
private:
	int startDialogID = 0;
	Dialogs dialogs;

public:
	Dialog();
	Dialog(const std::string& filename, bool fullPath = false);

	DialogLine& getLine(unsigned int index);
	Response getLineResponse(unsigned int index, unsigned responseID) const;
	int getStartDialogID() const;
	unsigned int getLineTextID(unsigned int index) const;
	unsigned int getSize() const;

	void addLine(DialogLine line);
	void removeLine(unsigned int index);
	void setLineResponse(unsigned int index, unsigned responseID, Response response);
	void setLineTextID(unsigned int index, unsigned int value);

	bool loadFromFile(const std::string& filename, bool fullPath = false);
	bool saveToFile(const std::string& filename, bool fullPath = false);
	bool load(std::ifstream& resource);
	void save(std::ofstream& resource);
};

#endif // DIALOG_HPP
