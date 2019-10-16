#pragma once

#include "Ray.ispc.h"
#include "Random.ispc.h"


struct Camera
{
	Vec3 Origin;
	Vec3 LowerLeftCorner;
	Vec3 Horizontal;
	Vec3 Vertical;

	Vec3 u, v, w;
	float LensRadius;
};

static inline Ray GetRay(const uniform Camera& camera, Random& rng, const float s, const float t)
{
	const Vec3 rd = camera.LensRadius * RandomInUnitDisk(rng);
	const Vec3 offset = camera.u * rd.x + camera.v * rd.y;

	return NewRay(camera.Origin + offset, camera.LowerLeftCorner + s * camera.Horizontal + t * camera.Vertical - camera.Origin - offset);
}
