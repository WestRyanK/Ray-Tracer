#include "RandomGenerator.h"

void RandomGenerator::Init()
{
	std::srand((unsigned int)time(NULL));
}

float RandomGenerator::RandomBetween(float min, float max)
{
	float range = max - min;
	float fractional = (float)std::rand() / RAND_MAX;

	float randomValue = min + fractional * range;
	return randomValue;
}
