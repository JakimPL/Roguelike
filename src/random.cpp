#include "random.hpp"

double Random::random()
{
	return double(rand() / RAND_MAX);
}
