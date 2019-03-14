#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#undef _USE_MATH_DEFINES

#include <iostream>

class Vec3 final
{
public:

	Vec3() = default;
	Vec3(const Vec3&) = default;
	Vec3(Vec3&&) = default;
	~Vec3() = default;
	Vec3& operator = (const Vec3&) = default;
	Vec3& operator = (Vec3&&) = default;

	explicit Vec3(const float a) : x_(a), y_(a), z_(a) { }
	Vec3(const float x, const float y, const float z) : x_(x), y_(y), z_(z) { }
	
	float x() const { return x_; }
	float y() const { return y_; }
	float z() const { return z_; }

	float r() const { return x_; }
	float g() const { return y_; }
	float b() const { return z_; }

	Vec3 const& operator + () const { return *this; }
	Vec3 operator - () const { return { -x_, -y_, -z_ }; }

	Vec3& operator += (const Vec3& v) { x_ += v.x_; y_ += v.y_; z_ += v.z_; return *this; }
	Vec3& operator -= (const Vec3& v) { x_ -= v.x_; y_ -= v.y_; z_ -= v.z_; return *this; }
	Vec3& operator *= (const Vec3& v) { x_ *= v.x_; y_ *= v.y_; z_ *= v.z_; return *this; }
	Vec3& operator /= (const Vec3& v) { x_ /= v.x_; y_ /= v.y_; z_ /= v.z_; return *this; }

	Vec3& operator += (const float s) { x_ += s; y_ += s; z_ += s; return *this; }
	Vec3& operator -= (const float s) { x_ -= s; y_ -= s; z_ -= s; return *this; }
	Vec3& operator *= (const float s) { x_ *= s; y_ *= s; z_ *= s; return *this; }
	Vec3& operator /= (const float s) { x_ /= s; y_ /= s; z_ /= s; return *this; }

private:

	float x_, y_, z_;
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

inline Vec3 Sqrt(const Vec3& v)
{
	return { std::sqrt(v.x()), std::sqrt(v.y()), std::sqrt(v.z()) };
}

inline Vec3 UnitVector(const Vec3& v)
{
	return v / Length(v);
}

inline std::istream& operator >> (std::istream& in, Vec3& v)
{
	float x, y, z;
	in >> x >> y >> z;
	v = Vec3(x, y, z);
	return in;
}

inline std::ostream& operator << (std::ostream& out, const Vec3& v)
{
	return out << v.x() << " " << v.y() << " " << v.z();
}
