#pragma once

#include "Vec3.ispc.h"

typedef RNGState Random;

// Generates a seed for a random number generator from 2 inputs plus a backoff
// https://github.com/nvpro-samples/optix_prime_baking/blob/master/random.h
// https://en.wikipedia.org/wiki/Tiny_Encryption_Algorithm
static inline unsigned int InitRandomSeed(unsigned int val0, unsigned int val1)
{
	unsigned int v0 = val0, v1 = val1, s0 = 0;

	for (unsigned int n = 0; n < 16; n++)
	{
		s0 += 0x9e3779b9;
		v0 += ((v1 << 4) + 0xa341316c) ^ (v1 + s0) ^ ((v1 >> 5) + 0xc8013ea4);
		v1 += ((v0 << 4) + 0xad90777d) ^ (v0 + s0) ^ ((v0 >> 5) + 0x7e95761e);
	}

	return v0;
}

static inline Random NewRandom(const int seed)
{
	Random rng;
	seed_rng(&rng, seed);
	return rng;
}

static inline float Uniform(Random& rng)
{
	return frandom(&rng);
}

static inline Vec3 RandomInUnitDisk(Random& rng)
{
	for (;;)
	{
		const Vec3 p = { 2 * Uniform(rng) - 1, 2 * Uniform(rng) - 1, 0 };
		if (SquaredLength(p) < 1)
		{
			return p;
		}
	}
}

static inline Vec3 RandomInUnitSphere(Random& rng)
{
	for (;;)
	{
		const Vec3 p = { 2 * Uniform(rng) - 1, 2 * Uniform(rng) - 1, 2 * Uniform(rng) - 1 };
		if (SquaredLength(p) < 1)
		{
			return p;
		}
	}
}
