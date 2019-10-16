#pragma once

#include "Vec3.ispc.h"

struct HitRecord
{
	float t;
	Vec3 Point;
	Vec3 Normal;
	int MaterialIndex;
};

static inline HitRecord NewHitRecord(float t, const Vec3& point, const Vec3& normal, const int materialIndex)
{
	const HitRecord hitRecord = { t, point, normal, materialIndex };
	return hitRecord;
}
