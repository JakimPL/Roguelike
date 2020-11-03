#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include "constants.hpp"
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

namespace Functions
{

bool compareHeaders(const char* header1, const char* header2);
const std::string getPath(const std::string& filename, Filetype filetype);
void read(std::ifstream& resource, char* string, unsigned int size);

};

#endif // FUNCTIONS_HPP
