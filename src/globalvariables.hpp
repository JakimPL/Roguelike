#ifndef GLOBALVARIABLES_HPP
#define GLOBALVARIABLES_HPP

#include <map>

enum GlobalVariable : unsigned int {
	g_NONE,
	g_TALKED_TO_JULIAN
};

struct GlobalState {
private:
	std::map<GlobalVariable, int> globalState;

public:
	int operator[](GlobalVariable variable) const;

	int getVariable(GlobalVariable variable) const;
	void setVariable(GlobalVariable variable, int value);
};

#endif // GLOBALVARIABLES_HPP
