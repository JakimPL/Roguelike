#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include "constants.hpp"
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
void read(std::ifstream& resource, char* string, unsigned int size);

};

#endif // FUNCTIONS_HPP
