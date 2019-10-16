#pragma once

typedef float<3> Vec3;

static inline float Dot(const Vec3& l, const Vec3& r)
{
	return l.x*r.x + l.y*r.y + l.z*r.z;
}

static inline Vec3 Cross(const Vec3& l, const Vec3& r)
{
	const Vec3 cross =
	{
		l.y*r.z - l.z*r.y,
		l.z*r.x - l.x*r.z,
		l.x*r.y - l.y*r.x
	};

	return cross;
}

static inline float SquaredLength(const Vec3& v)
{
	return Dot(v, v);
}

static inline float Length(const Vec3& v)
{
	return sqrt(SquaredLength(v));
}

static inline Vec3 Sqrt(const Vec3& v)
{
	const Vec3 s = { sqrt(v.x), sqrt(v.y), sqrt(v.z) };
	return s;
}

static inline Vec3 UnitVector(const Vec3& v)
{
	return v / Length(v);
}
