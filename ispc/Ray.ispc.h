#pragma once

#include "Vec3.ispc.h"

struct Ray
{
	Vec3 Origin;
	Vec3 Direction;
};

static inline Vec3 PointAtParameter(const Ray& ray, const float t)
{
	return ray.Origin + t * ray.Direction;
}