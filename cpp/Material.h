#pragma once

#include "HitRecord.h"
#include "Random.h"
#include "Ray.h"
#include "Vec3.h"
#include <optional>

struct MaterialRay
{
	MaterialRay(const Ray& scattered, const Vec3& attenuation) :
		Scattered(scattered), Attenuation(attenuation)
	{
	}

	const Ray Scattered;
	const Vec3 Attenuation;
};

class Material
{
public:

	Material() = default;
	Material(const Material&) = default;
	Material(Material&&) = default;
	virtual ~Material() = default;
	Material& operator = (const Material&) = delete;
	Material& operator = (Material&&) = delete;

	virtual std::optional<MaterialRay> Scatter(const Ray& ray, const HitRecord& record, Random& random) const = 0;

protected:

	static Vec3 Reflect(const Vec3& v, const Vec3& normal)
	{
		return v - 2 * Dot(v, normal)*normal;
	}

	static std::optional<Vec3> Refract(const Vec3& v, const Vec3& normal, const float niOverNt)
	{
		const Vec3 uv = UnitVector(v);
		const float dt = Dot(uv, normal);
		const float discriminant = 1 - niOverNt * niOverNt*(1 - dt * dt);

		return discriminant > 0
			? niOverNt * (uv - normal * dt) - normal * std::sqrt(discriminant)
			: std::optional<Vec3>();
	}
};