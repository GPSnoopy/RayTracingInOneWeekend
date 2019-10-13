#pragma once

#include "Vec3.h"
#include <random>

typedef std::mt19937 Random;

inline float Uniform(Random& random)
{
	return std::uniform_real_distribution<float>()(random);
}

inline Vec3 RandomInUnitDisk(Random& random)
{
	for (;;)
	{
		const Vec3 p = 2 * Vec3(Uniform(random), Uniform(random), 0) - Vec3(1, 1, 0);
		if (SquaredLength(p) < 1)
		{
			return p;
		}
	}
}

inline Vec3 RandomInUnitSphere(Random& random)
{
	for (;;)
	{
		const Vec3 p = 2 * Vec3(Uniform(random), Uniform(random), Uniform(random)) - 1;
		if (SquaredLength(p) < 1)
		{
			return p;
		}
	}
}
