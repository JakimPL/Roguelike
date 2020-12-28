#include "random.hpp"

#include "log.hpp"

double Random::random()
{
	return double(rand()) / RAND_MAX;
}

double Random::random(double max)
{
	return Random::random() * max;
}

double Random::random(double min, double max)
{
	return Random::random(max - min) + min;
}
