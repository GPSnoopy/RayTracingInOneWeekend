#pragma once

#include "HitRecord.h"
#include "Ray.h"
#include <optional>

class Hittable
{
public:

	Hittable() = default;
	Hittable(const Hittable&) = default;
	Hittable(Hittable&&) = default;
	virtual ~Hittable() = default;

	Hittable& operator = (const Hittable&) = default;
	Hittable& operator = (Hittable&&) = default;

	virtual std::optional<HitRecord> Hit(const Ray& ray, float tMin, float tMax) const = 0;
};
