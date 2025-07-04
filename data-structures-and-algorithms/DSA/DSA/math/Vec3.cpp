#include "Vec3.h"

Vec3::Vec3()
	: x(0), y(0), z(0)
{
}

Vec3::Vec3(float x, float y, float z)
	: x(x), y(y), z(z)
{
}

Vec3& Vec3::operator=(const Vec3& other)
{
	x = other.x;
	y = other.y;
	z = other.z;
	return *this;
}

Vec3 Vec3::operator+(const Vec3& other) const
{
	return Vec3(x + other.x, y + other.y, z + other.z);
}

Vec3 Vec3::operator-(const Vec3& other) const
{
	return Vec3(x - other.x, y - other.y, z - other.z);
}

Vec3 Vec3::operator*(const float other) const
{
	return Vec3(x * other, y * other, z * other);
}

Vec3 Vec3::operator/(const float other) const
{
	return Vec3(x / other, y / other, z / other);
}

Vec3& Vec3::operator+=(const Vec3& other)
{
	x += other.x;
	y += other.y;
	z += other.z;
	return *this;
}

Vec3& Vec3::operator-=(const Vec3& other)
{
	x -= other.y;
	y -= other.y;
	z -= other.z;
	return *this;
}

Vec3& Vec3::operator*=(const float other)
{
	x *= other;
	y *= other;
	z *= other;
	return *this;
}

Vec3& Vec3::operator/=(const float other)
{
	x /= other;
	y /= other;
	z /= other;
	return *this;
}
