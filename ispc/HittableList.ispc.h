#pragma once

#include "Hittable.ispc.h"

static inline int Hit(const Hittable* hittables, const int length, const Ray& ray, const float tMin, float tMax, HitRecord& hitRecord)
{
	int hasRecord = false;

	for (int i = 0; i < length; ++i)
	{
		const Hittable hittable = hittables[i];
		
		if (Hit(hittable, ray, tMin, tMax, hitRecord))
		{
			tMax = hitRecord.t;
			hasRecord = true;
		}
	}

	return hasRecord;
}
