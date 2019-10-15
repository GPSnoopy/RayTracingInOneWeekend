#pragma once

#include "Vec3.ispc.h"

struct Ray
{
	Vec3 Origin;
	Vec3 Direction;
};

static inline Ray NewRay(const Vec3& origin, const Vec3& direction)
{
	const Ray ray = { origin, direction };
	return ray;
}

static inline Vec3 PointAtParameter(const Ray& ray, const float t)
{
	return ray.Origin + t * ray.Direction;
}