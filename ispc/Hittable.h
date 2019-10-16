#pragma once

#include "Vec3.h"

class Hittable final
{
public:

	static ispc::Hittable Sphere(const Vec3& centre, const float radius, const int materialIndex)
	{
		return { ispc::HittableType::Sphere, centre, radius, materialIndex };
	}
};
