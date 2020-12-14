#include "globalvariables.hpp"

#include <stdexcept>

int GlobalState::getVariable(GlobalVariable variable) const
{
	try {
		return globalState.at(variable);
	}  catch (std::out_of_range&) {
		return 0;
	}
}

void GlobalState::setVariable(GlobalVariable variable, int value)
{
	globalState[variable] = value;
}

int GlobalState::operator[](GlobalVariable variable) const
{
	return getVariable(variable);
}
