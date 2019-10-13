#pragma once

#include "Material.h"

class Metal final : public Material
{
public:

	Metal(const Vec3& albedo, const float fuzz)
		: Albedo(albedo), Fuzz(std::min(std::max(0.0f, fuzz), 1.0f))
	{
	}

	std::optional<MaterialRay> Scatter(const Ray& ray, const HitRecord& record, Random& random) const override
	{
		const Vec3 reflected = Reflect(UnitVector(ray.Direction), record.Normal);

		return Dot(reflected, record.Normal) > 0
			? MaterialRay(Ray(record.Point, reflected + Fuzz*RandomInUnitSphere(random)), Albedo)
			: std::optional<MaterialRay>();
	}

	const Vec3 Albedo;
	const float Fuzz;
};
