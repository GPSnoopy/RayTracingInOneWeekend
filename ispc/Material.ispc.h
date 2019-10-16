#pragma once

#include "HitRecord.ispc.h"
#include "Random.ispc.h"
#include "Ray.ispc.h"

// Material Definitions

enum MaterialType
{
	Lambertian = 0,
	Metallic = 1,
	Dielectric = 2
};

struct Material
{
	MaterialType TypeId; // Which material are we dealing with
	Vec3 Albedo; // Base material
	float Fuzziness; // Metal fuzziness
	float RefractionIndex; // Dielectric refraction index
};

struct MaterialRay
{
	Ray Scattered;
	Vec3 Attenuation;
};

// Utility Functions

static inline MaterialRay NewMaterialRay(const Vec3 origin, const Vec3& direction, const Vec3& attenuation)
{
	const MaterialRay materialRay = { {origin, direction}, attenuation };
	return materialRay;
}

static inline Vec3 Reflect(const Vec3& v, const Vec3& normal)
{
	return v - 2 * Dot(v, normal) * normal;
}

static inline int Refract(const Vec3& v, const Vec3& normal, const float niOverNt, Vec3& refactedRay)
{
	const Vec3 uv = UnitVector(v);
	const float dt = Dot(uv, normal);
	const float discriminant = 1 - niOverNt * niOverNt * (1 - dt * dt);

	if (discriminant <= 0)
	{
		return false;
	}

	refactedRay = niOverNt * (uv - normal * dt) - normal * sqrt(discriminant);
	return true;
}

static inline float Schlick(const float cosine, const float refractionIndex)
{
	float r0 = (1 - refractionIndex) / (1 + refractionIndex);
	r0 *= r0;
	return r0 + (1 - r0) * pow(1 - cosine, 5);
}

// Scatter Functions

static inline int DielectricScatter(const Material& material, const Ray& ray, const HitRecord& record, Random& rng, MaterialRay& materialRay)
{
	assert(material.TypeId == Dielectric);

	const float dot = Dot(ray.Direction, record.Normal);

	const Vec3 one = { 1, 1, 1 };
	const Vec3 outwardNormal = dot > 0 ? -record.Normal : record.Normal;
	const float niOverNt = dot > 0 ? material.RefractionIndex : 1 / material.RefractionIndex;
	const float cosine = dot > 0 ? material.RefractionIndex * dot / Length(ray.Direction) : -dot / Length(ray.Direction);

	Vec3 refractedRay;
	const int refracted = Refract(ray.Direction, outwardNormal, niOverNt, refractedRay);
	const float reflectProb = refracted ? Schlick(cosine, material.RefractionIndex) : 1;

	materialRay = Uniform(rng) < reflectProb
		? NewMaterialRay(record.Point, Reflect(ray.Direction, record.Normal), one)
		: NewMaterialRay(record.Point, refractedRay, one);

	return true;
}

static inline int LambertianScatter(const Material& material, const Ray&, const HitRecord& record, Random& rng, MaterialRay& materialRay)
{
	assert(material.TypeId == Lambertian);
	
	const Vec3 target = record.Point + record.Normal + RandomInUnitSphere(rng);
	materialRay = NewMaterialRay(record.Point, target - record.Point, material.Albedo);

	return true;
}

static inline int MetallicScatter(const Material& material, const Ray& ray, const HitRecord& record, Random& rng, MaterialRay& materialRay)
{
	assert(material.TypeId == Metallic);
	
	const Vec3 reflected = Reflect(UnitVector(ray.Direction), record.Normal);

	if (Dot(reflected, record.Normal) <= 0)
	{
		return false;
	}

	materialRay = NewMaterialRay(record.Point, reflected + material.Fuzziness * RandomInUnitSphere(rng), material.Albedo);
	return true;
}

static inline int Scatter(const Material& material, const Ray& ray, const HitRecord& record, Random& rng, MaterialRay& materialRay)
{
	switch (material.TypeId)
	{
	case Dielectric:
		return DielectricScatter(material, ray, record, rng, materialRay);
	case Lambertian:
		return LambertianScatter(material, ray, record, rng, materialRay);
	case Metallic:
		return MetallicScatter(material, ray, record, rng, materialRay);
	default:
		assert(false);
	}
}
