#pragma once

#include "Material.h"

class Lambertian final : public Material
{
public:

	explicit Lambertian(const Vec3& albedo)
		: Albedo(albedo)
	{
	}

	std::optional<MaterialRay> Scatter(const Ray&, const HitRecord& record, Random& random) const override
	{
		const Vec3 target = record.Point + record.Normal + RandomInUnitSphere(random);
		return MaterialRay(Ray(record.Point, target - record.Point), Albedo);
	}

	const Vec3 Albedo;
};
