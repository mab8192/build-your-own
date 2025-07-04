#pragma once

struct Vec2
{
	float x, y;

	Vec2();
	Vec2(float x, float y);

	Vec2& operator=(const Vec2& other);

	Vec2 operator+(const Vec2& other) const;
	Vec2 operator-(const Vec2& other) const;
	Vec2 operator*(const float other) const;
	Vec2 operator/(const float other) const;

	Vec2& operator+=(const Vec2& other);
	Vec2& operator-=(const Vec2& other);
	Vec2& operator*=(const float other);
	Vec2& operator/=(const float other);
};