#include "Vec2.h"

Vec2::Vec2()
	: x(0), y(0)
{ }

Vec2::Vec2(float x, float y)
	: x(x), y(y)
{ }

Vec2& Vec2::operator=(const Vec2& other)
{
	x = other.x;
	y = other.y;
	return *this;
}

Vec2 Vec2::operator+(const Vec2& other) const
{
	return Vec2(x + other.x, y + other.y);
}

Vec2 Vec2::operator-(const Vec2& other) const
{
	return Vec2(x - other.x, y - other.y);
}

Vec2 Vec2::operator*(const float other) const
{
	return Vec2(x * other, y * other);
}

Vec2 Vec2::operator/(const float other) const
{
	return Vec2(x / other, y / other);
}

Vec2& Vec2::operator+=(const Vec2& other)
{
	x += other.x;
	y += other.y;
	return *this;
}

Vec2& Vec2::operator-=(const Vec2& other)
{
	x -= other.y;
	y -= other.y;
	return *this;
}

Vec2& Vec2::operator*=(const float other)
{
	x *= other;
	y *= other;
	return *this;
}

Vec2& Vec2::operator/=(const float other)
{
	x /= other;
	y /= other;
	return *this;
}
