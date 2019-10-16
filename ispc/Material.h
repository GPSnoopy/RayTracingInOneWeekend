#pragma once

#include "Vec3.h"

class Material final
{
public:

	static ispc::Material Lambertian(const Vec3& diffuse)
	{
		return { ispc::MaterialType::Lambertian, diffuse, 0.0f, 0.0f };
	}

	static ispc::Material Metallic(const Vec3& diffuse, const float fuzziness)
	{
		return { ispc::MaterialType::Metallic, diffuse, fuzziness, 0.0f };
	}

	static ispc::Material Dielectric(const float refractionIndex)
	{
		return { ispc::MaterialType::Dielectric, Vec3(0.7f, 0.7f, 1.0f),  0.0f, refractionIndex };
	}
};
