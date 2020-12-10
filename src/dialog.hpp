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
	isGreaterThan
};

struct DialogCondition {
	GlobalVariable variable;
	ComparisonOperator comparisonOperator;
	bool compareWithVariable;
	int value;
};

struct DialogAction {
	unsigned int nextDialogID;
	GlobalVariable variable;
	int value;
};

struct Response {
	unsigned int textID;
	DialogCondition condition;
	DialogAction action;
};

typedef std::vector<Response> Responses;

struct DialogLine {
	unsigned int textID;
	Responses responses;
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

	DialogLine& getLine(unsigned int);
	unsigned int getSize() const;

	void addLine(DialogLine line);
	void removeLine(unsigned int index);

	bool loadFromFile(const std::string& filename, bool fullPath = false);
	bool saveToFile(const std::string& filename, bool fullPath = false);
	bool load(std::ifstream& resource);
	void save(std::ofstream& resource);
};

#endif // DIALOG_HPP
