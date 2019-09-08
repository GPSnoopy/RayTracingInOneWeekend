#pragma once

#include "Material.h"

class Dielectric final : public Material
{
public:

	explicit Dielectric(const float refractionIndex)
		: RefractionIndex(refractionIndex)
	{
	}

	std::optional<MaterialRay> Scatter(const Ray& ray, const HitRecord& record, Random& random) const override
	{
		const float dot = Dot(ray.Direction, record.Normal);

		const Vec3 outwardNormal = dot > 0 ? -record.Normal : record.Normal;
		const float niOverNt = dot > 0 ? RefractionIndex : 1 / RefractionIndex;
		const float cosine = dot > 0 ? RefractionIndex * dot / Length(ray.Direction) : -dot / Length(ray.Direction);

		const auto refracted = Refract(ray.Direction, outwardNormal, niOverNt);
		const float reflectProb = refracted ? Schlick(cosine, RefractionIndex) : 1;

		return Uniform(random) < reflectProb
			? MaterialRay(Ray(record.Point, Reflect(ray.Direction, record.Normal)), Vec3(1))
			: MaterialRay(Ray(record.Point, *refracted), Vec3(1));
	}

	const float RefractionIndex;

private:

	// Polynomial approximation by Christophe Schlick
	static float Schlick(const float cosine, const float refractionIndex)
	{
		float r0 = (1 - refractionIndex) / (1 + refractionIndex);
		r0 *= r0;
		return r0 + (1 - r0) * std::pow(1 - cosine, 5);
	}
};
