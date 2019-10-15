#pragma once

#include <random>

typedef std::mt19937 Random;

inline float Uniform(Random& random)
{
	return std::uniform_real_distribution<float>()(random);
}
