#pragma once

#include "Hittable.h"

class Sphere final : public Hittable
{
public:

	Sphere(const Vec3& center, const float radius, const Material* const material)
		: Center(center), Radius(radius), Material(material)
	{
	}

	const Vec3 Center;
	const float Radius;
	const std::unique_ptr<const class Material> Material;

	std::optional<HitRecord> Hit(const Ray& ray, const float tMin, const float tMax) const override
	{
		// https://en.wikipedia.org/wiki/Quadratic_formula

		const Vec3 oc = ray.Origin - Center;
		const float a = Dot(ray.Direction, ray.Direction);
		const float b = Dot(oc, ray.Direction);
		const float c = Dot(oc, oc) - Radius * Radius;
		const float discriminant = b * b - a * c;

		if (discriminant >= 0)
		{
			const float t1 = (-b - std::sqrt(discriminant)) / a;
			const float t2 = (-b + std::sqrt(discriminant)) / a;

			if ((tMin <= t1 && t1 < tMax) || (tMin <= t2 && t2 < tMax))
			{
				const float t = (tMin <= t1 && t1 < tMax) ? t1 : t2;
				const Vec3 point = ray.PointAtParameter(t);
				const Vec3 normal = (point - Center) / Radius;

				return HitRecord(t, point, normal, Material.get());
			}
		}

		return std::nullopt;
	}
};
