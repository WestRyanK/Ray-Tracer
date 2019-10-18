#pragma once
#include <time.h>
#include <random>

class RandomGenerator
{
public:
	static void Init();
	static float RandomBetween(float min, float max);
};
