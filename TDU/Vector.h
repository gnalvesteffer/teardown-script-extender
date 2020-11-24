#pragma once
#include <limits>

class Vector2
{
public:
	float x, y;
	Vector2(float _x, float _y)
	{
		x = _x; y = _y;
	}

	Vector2()
	{
		x = 0; y = 0;
	}

	void Inverse()
	{
		x = -x;
		y = -y;
	}

	__forceinline Vector2&	operator+=(const Vector2 &v);
	Vector2	operator+(const Vector2& v) const;
};

inline Vector2 Vector2::operator+(const Vector2& v) const {
	Vector2 res;
	res.x = x + v.x;
	res.y = y + v.y;
	return res;
}

__forceinline  Vector2& Vector2::operator+=(const Vector2& v) {
	x += v.x; y += v.y;
	return *this;
}


class Vector3
{
public:
	float x, y, z;
	Vector3(float _x, float _y, float _z)
	{
		x = _x; y = _y; z = _z;
	}

	Vector3()
	{
		x = 0; y = 0; z = 0;
	}

	void Inverse()
	{
		x = -x;
		y = -y;
		z = -z;
	}
	__inline bool IsValid() const
	{
		return std::isfinite(x) && std::isfinite(y) && std::isfinite(z);
	}
	__forceinline Vector3&	operator+=(const Vector3 &v);
	__forceinline Vector3&	operator-=(const Vector3 &v);
	__forceinline Vector3&	operator*=(float s);

	Vector3	operator+(const Vector3& v) const;
	Vector3	operator-(const Vector3& v) const;
};

inline Vector3 Vector3::operator-(const Vector3& v) const {
	Vector3 res;
	res.x = x - v.x;
	res.y = y - v.y;
	res.z = z - v.z;
	return res;
}
inline Vector3 Vector3::operator+(const Vector3& v) const {
	Vector3 res;
	res.x = x + v.x;
	res.y = y + v.y;
	res.z = z + v.z;
	return res;
}

__forceinline  Vector3& Vector3::operator*=(float fl) {
	x *= fl;
	y *= fl;
	z *= fl;
	return *this;
}

__forceinline  Vector3& Vector3::operator+=(const Vector3& v) {
	x += v.x; y += v.y; z += v.z;
	return *this;
}

__forceinline  Vector3& Vector3::operator-=(const Vector3& v) {
	x -= v.x; y -= v.y; z -= v.z;
	return *this;
}

class Vector4
{
public:
	float x, y, z, w;

	Vector4(float _x, float _y, float _z, float _w)
	{
		x = _x; y = _y; z = _z; w = _w;
	}

	Vector4()
	{
		x = 0; y = 0; z = 0; w = 0;
	}

	void Inverse()
	{
		x = -x;
		y = -y;
		z = -z;
		w = -w;
	}
};