#pragma once

#include "Renderer_ispc.h"

#define _USE_MATH_DEFINES
#include <cmath>
#undef _USE_MATH_DEFINES

class Vec3 final
{
public:

	Vec3() = default;
	Vec3(const Vec3&) = default;
	Vec3(Vec3&&) = default;
	~Vec3() = default;
	Vec3& operator = (const Vec3&) = default;
	Vec3& operator = (Vec3&&) = default;

	explicit Vec3(const float a) : f_{a, a, a} { }
	Vec3(const float x, const float y, const float z) : f_{ x, y, z } { }

	const ispc::float3& array() const { return f_; }
	float x() const { return f_.v[0]; }
	float y() const { return f_.v[1]; }
	float z() const { return f_.v[2]; }

	operator ispc::float3() const { return array(); }

	Vec3 const& operator + () const { return *this; }
	Vec3 operator - () const { return { -x(), -y(), -z() }; }

	Vec3& operator += (const Vec3& v) { f_.v[0] += v.x(); f_.v[1] += v.y(); f_.v[2] += v.z(); return *this; }
	Vec3& operator -= (const Vec3& v) { f_.v[0] -= v.x(); f_.v[1] -= v.y(); f_.v[2] -= v.z(); return *this; }
	Vec3& operator *= (const Vec3& v) { f_.v[0] *= v.x(); f_.v[1] *= v.y(); f_.v[2] *= v.z(); return *this; }
	Vec3& operator /= (const Vec3& v) { f_.v[0] /= v.x(); f_.v[1] /= v.y(); f_.v[2] /= v.z(); return *this; }

	Vec3& operator += (const float s) { f_.v[0] += s; f_.v[1] += s; f_.v[2] += s; return *this; }
	Vec3& operator -= (const float s) { f_.v[0] -= s; f_.v[1] -= s; f_.v[2] -= s; return *this; }
	Vec3& operator *= (const float s) { f_.v[0] *= s; f_.v[1] *= s; f_.v[2] *= s; return *this; }
	Vec3& operator /= (const float s) { f_.v[0] /= s; f_.v[1] /= s; f_.v[2] /= s; return *this; }

private:

	ispc::float3 f_;
};

inline Vec3 operator + (const Vec3& left, const Vec3& right) { return Vec3(left) += right; }
inline Vec3 operator - (const Vec3& left, const Vec3& right) { return Vec3(left) -= right; }
inline Vec3 operator * (const Vec3& left, const Vec3& right) { return Vec3(left) *= right; }
inline Vec3 operator / (const Vec3& left, const Vec3& right) { return Vec3(left) /= right; }

inline Vec3 operator + (const Vec3& left, const float right) { return Vec3(left) += right; }
inline Vec3 operator - (const Vec3& left, const float right) { return Vec3(left) -= right; }
inline Vec3 operator * (const Vec3& left, const float right) { return Vec3(left) *= right; }
inline Vec3 operator / (const Vec3& left, const float right) { const float f = 1.f / right; return Vec3(left) *= f; }

inline Vec3 operator + (const float left, const Vec3& right) { return { left + right.x(), left + right.y(), left + right.z() }; }
inline Vec3 operator - (const float left, const Vec3& right) { return { left - right.x(), left - right.y(), left - right.z() }; }
inline Vec3 operator * (const float left, const Vec3& right) { return { left * right.x(), left * right.y(), left * right.z() }; }
inline Vec3 operator / (const float left, const Vec3& right) { return { left / right.x(), left / right.y(), left / right.z() }; }

inline float Dot(const Vec3& l, const Vec3& r)
{
	return l.x()*r.x() + l.y()*r.y() + l.z()*r.z();
}

inline Vec3 Cross(const Vec3& l, const Vec3& r)
{
	return Vec3
	{
		l.y()*r.z() - l.z()*r.y(),
		l.z()*r.x() - l.x()*r.z(),
		l.x()*r.y() - l.y()*r.x()
	};
}

inline float SquaredLength(const Vec3& v)
{
	return Dot(v, v);
}

inline float Length(const Vec3& v)
{
	return std::sqrt(SquaredLength(v));
}

inline Vec3 UnitVector(const Vec3& v)
{
	return v / Length(v);
}
