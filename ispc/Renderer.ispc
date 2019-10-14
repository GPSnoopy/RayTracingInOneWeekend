
#include "Hittable.ispc.h"
#include "HittableList.ispc.h"
#include "Material.ispc.h"

export uniform int GetTarget()
{
#if defined(ISPC_TARGET_SSE2)
	return 0;
#elif defined(ISPC_TARGET_SSE4)
	return 1;
#elif defined(ISPC_TARGET_AVX)
	return 2;
#elif defined(ISPC_TARGET_AVX2)
	return 3;
#elif defined(ISPC_TARGET_AVX2SKX)
	return 4;
#else
	return -1;
#endif
}

Vec3 Color(const Hittable* hittables, const int length, const Ray& ray, const int depth, const int maxDepth, Random& rng)
{
	const Vec3 zero = { 0, 0, 0 };

	HitRecord hitRecord;
	const int hasHit = Hit(hittables, length, ray, 0.001f, 1e30f, hitRecord);

	if (hasHit)
	{
		MaterialRay materialRay;
		const Material material = *((Material*) hitRecord.MaterialRef);
		const int isScattered = Scatter(material, ray, hitRecord, rng, materialRay);
		
		return depth < maxDepth && isScattered
			? materialRay.Attenuation * Color(hittables, length, materialRay.Scattered, depth + 1, maxDepth, rng)
			: zero;
	}

	const Vec3 direction = UnitVector(ray.Direction);
	const float t = 0.5f * (direction.y + 1);
	const Vec3 skyWhite = { 1.0f, 1.0f, 1.0f };
	const Vec3 skyColor = { 0.5f, 0.7f, 1.0f };
	return (1 - t) * skyWhite + t * skyColor;
}

export uniform int GetWidth()
{
	return programCount;
}

export void Render(uniform int width, uniform int height)
{

}
