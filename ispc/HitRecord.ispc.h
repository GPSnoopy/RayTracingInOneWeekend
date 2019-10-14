#pragma once

#include "Vec3.ispc.h"

struct HitRecord
{
	float t;
	Vec3 Point;
	Vec3 Normal;
	const void* MaterialRef;
};

static inline HitRecord NewHitRecord(float t, const Vec3& point, const Vec3& normal, const void* materialRef)
{
	const HitRecord hitRecord = { t, point, normal, materialRef };
	return hitRecord;
}

