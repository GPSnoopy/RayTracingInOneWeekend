#pragma once

#include "Vec3.h"

class Camera final
{
public:

	static ispc::Camera LookAt(
		const Vec3& origin, const Vec3& target,
		const Vec3& up,
		const float verticalFov, const float aspectRatio,
		const float aperture, const float focusDistance)
	{
		const float theta = verticalFov * float(M_PI / 180);
		const float halfHeight = std::tan(theta / 2);
		const float halfWidth = aspectRatio * halfHeight;
		const float lensRadius = aperture / 2;

		const Vec3 w = UnitVector(origin - target);
		const Vec3 u = UnitVector(Cross(up, w));
		const Vec3 v = Cross(w, u);

		const Vec3 lowerLeftCorner = origin - focusDistance * (halfWidth * u + halfHeight * v + w);
		const Vec3 horizontal = 2 * halfWidth * focusDistance * u;
		const Vec3 vertical = 2 * halfHeight * focusDistance * v;

		return {
			origin,
			lowerLeftCorner,
			horizontal,
			vertical,
			u, v, w,
			lensRadius
		};
	}
};
