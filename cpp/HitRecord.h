#pragma once

#include "Vec3.h"

class Material;

struct HitRecord final
{
	HitRecord(const float t, const Vec3& point, const Vec3& normal, const Material* material) :
		t(t), Point(point), Normal(normal), Material(material)
	{
	}

	float t;
	Vec3 Point;
	Vec3 Normal;
	const class Material* Material;
};
