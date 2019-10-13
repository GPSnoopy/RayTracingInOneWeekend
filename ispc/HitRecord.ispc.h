#pragma once

#include "Vec3.ispc.h"

struct Material;

struct HitRecord
{
	float t;
	Vec3 Point;
	Vec3 Normal;
	const Material* MaterialRef;
};
