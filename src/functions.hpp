#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include "constants.hpp"
#include "options.hpp"
#include <string>

#include <boost/program_options.hpp>

namespace bpo = boost::program_options;

namespace Functions
{

bpo::options_description addProgramDescription();
bool compareHeaders(const char* header1, const char* header2);
const std::string getPath(const std::string& filename, Filetype filetype);
bpo::variables_map getVariablesMap(bpo::options_description description, int argc, char *argv[]);
std::vector<Mode> parseProgramArguments(int argc, char *argv[]);
std::vector<std::string> parseString(const std::string& string, const std::string& delimiter = " ", unsigned int lineWidth = options.dialog.lineWidth);
void read(std::ifstream& resource, char* string, unsigned int size);
std::string readString(std::ifstream& resource, unsigned int size);

template <typename type>
static void increase(type& variable, int max)
{
	variable = std::min(max, variable + 1);
};

template <typename type>
static void decrease(type& variable, int min = 0)
{
	variable = std::max(min, variable - 1);
};

template <typename type>
static void add(type& variable, type value, int max)
{
	variable = std::min(max, variable + value);
};

template <typename type>
static void subtract(type& variable, type value, int min = 0)
{
	variable = std::max(min, variable - value);
};

template <typename type>
static void clamp(type& variable, int min, int max)
{
	variable = std::max(min, std::min(max, variable));
}

};

#endif // FUNCTIONS_HPP
