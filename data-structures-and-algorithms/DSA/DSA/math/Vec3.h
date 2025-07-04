#pragma once

struct Vec3
{
	float x, y, z;

	Vec3();
	Vec3(float x, float y, float z);

	Vec3& operator=(const Vec3& other);

	Vec3 operator+(const Vec3& other) const;
	Vec3 operator-(const Vec3& other) const;
	Vec3 operator*(const float other) const;
	Vec3 operator/(const float other) const;

	Vec3& operator+=(const Vec3& other);
	Vec3& operator-=(const Vec3& other);
	Vec3& operator*=(const float other);
	Vec3& operator/=(const float other);
};