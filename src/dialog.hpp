#ifndef DIALOG_HPP
#define DIALOG_HPP

#include "globalvariables.hpp"

#include <vector>

enum class ComparisonOperator {
	isEqual,
	isDifferent,
	isLessOrEqualThan,
	isLessThan,
	isGreaterOrEqualThan,
	isGreaterThan

};

struct Response {
	unsigned int textID;
	struct Condition {
		GlobalVariable variable;
		ComparisonOperator comparisonOperator;
		bool compareWithVariable;
		int value;
	} condition;
	struct Action {
		unsigned int nextDialogID;
		GlobalVariable variable;
		int value;
	} action;
};

class Dialog
{
private:
	int dialogID;
	unsigned int textID;
	std::vector<Response> responses;

public:
	Dialog();
};

#endif // DIALOG_HPP
