
#include "Camera.ispc.h"
#include "HittableList.ispc.h"
#include "Material.ispc.h"

static Vec3 Color(
	Random& rng,
	const uniform Hittable hittables[], const uniform int length,
	const uniform Material materials[],
	const Ray& ray, 
	const int depth, 
	const uniform int maxDepth)
{
	HitRecord hitRecord;
	const int hasHit = Hit(hittables, length, ray, 0.001f, 1e30f, hitRecord);

	if (hasHit)
	{
		MaterialRay materialRay;

		#pragma ignore warning(perf)
		const Material material = materials[hitRecord.MaterialIndex];
		const int isScattered = Scatter(material, ray, hitRecord, rng, materialRay);
		const Vec3 zero = { 0, 0, 0 };
				
		return depth < maxDepth && isScattered
			? materialRay.Attenuation * Color(rng, hittables, length, materials, materialRay.Scattered, depth + 1, maxDepth)
			: zero;
	}

	const Vec3 direction = UnitVector(ray.Direction);
	const float t = 0.5f * (direction.y + 1);
	const Vec3 skyWhite = { 1.0f, 1.0f, 1.0f };
	const Vec3 skyColor = { 0.5f, 0.7f, 1.0f };
	return (1 - t) * skyWhite + t * skyColor;
}

export static void RenderTile(
	uniform Vec3 buffer[],
	const uniform Hittable hittables[], const uniform int length,
	const uniform Material materials[],
	const uniform Camera& camera,
	const uniform int x0, const uniform int x1,
	const uniform int y0, const uniform int y1,
	const uniform int w, const uniform int h,
	const uniform int samples, 
	const uniform int bounces)
{
	foreach_tiled(y = y0 ... y1, x = x0 ... x1) 
	{
		Random rng = NewRandom(InitRandomSeed(y, x));

		Vec3 color = { 0, 0, 0 };

		for (int s = 0; s < samples; ++s)
		{
			const float u = (float) (x + Uniform(rng)) / (float) (w);
			const float v = (float) (y + Uniform(rng)) / (float) (h);

			color += Color(rng, hittables, length, materials, GetRay(camera, rng, u, v), 0, bounces);
		}

		#pragma ignore warning(perf)
		buffer[y * w + x] = Sqrt(color / (float) samples) * 255.99f;
	}
}

export uniform int GetVersionMajor()
{
	return ISPC_MAJOR_VERSION;
}

export uniform int GetVersionMinor()
{
	return ISPC_MINOR_VERSION;
}

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
#elif defined(ISPC_TARGET_AVX512SKX)
	return 4;
#elif defined(ISPC_TARGET_NEON)
	return 100;
#else
#error Unknown ISPC target
#endif
}

export uniform int GetTargetWidth()
{
	return TARGET_WIDTH;
}

export uniform int GetTargetElementWidth()
{
	return TARGET_ELEMENT_WIDTH;
}
