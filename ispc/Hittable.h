#pragma once

#include "Vec3.h"

class Hittable final
{
public:

	static ispc::Hittable Sphere(const Vec3& centre, const float radius, const ispc::Material& material)
	{
		return { ispc::HittableType_t::Sphere, centre, radius, material };
	}
};
