#pragma once

#include "Material.ispc.h"
#include "Ray.ispc.h"

enum HittableType
{
	Sphere = 0,
};

struct Hittable
{
	HittableType TypeId; // Which hittable are we dealing with
	Vec3 Center; // Sphere center
	float Radius; // Sphere radius
	int MaterialIndex; // Material index
};

static inline int SphereHit(const Hittable& sphere, const Ray& ray, const float tMin, const float tMax, HitRecord& hitRecord)
{
	assert(sphere.TypeId == Sphere);
	
	// https://en.wikipedia.org/wiki/Quadratic_formula

	const Vec3 oc = ray.Origin - sphere.Center;
	const float a = Dot(ray.Direction, ray.Direction);
	const float b = Dot(oc, ray.Direction);
	const float c = Dot(oc, oc) - sphere.Radius * sphere.Radius;
	const float discriminant = b * b - a * c;

	if (discriminant >= 0)
	{
		const float t1 = (-b - sqrt(discriminant)) / a;
		const float t2 = (-b + sqrt(discriminant)) / a;

		if ((tMin <= t1 && t1 < tMax) || (tMin <= t2 && t2 < tMax))
		{
			const float t = (tMin <= t1 && t1 < tMax) ? t1 : t2;
			const Vec3 point = PointAtParameter(ray, t);
			const Vec3 normal = (point - sphere.Center) / sphere.Radius;

			hitRecord = NewHitRecord(t, point, normal, sphere.MaterialIndex);
			return true;
		}
	}

	return false;
}

static inline int Hit(const Hittable& hittable, const Ray& ray, const float tMin, const float tMax, HitRecord& hitRecord)
{
	switch (hittable.TypeId)
	{
	case Sphere:
		return SphereHit(hittable, ray, tMin, tMax, hitRecord);
	default:
		assert(false);
	}
}