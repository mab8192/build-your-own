#include "Vector.h"
#include "math.h"

namespace Ark {
	Vector::Vector()
		: _x(0), _y(0)
	{
	}

	Vector::Vector(float val)
		: _x(val), _y(val)
	{
	}

	Vector::Vector(float x, float y)
		: _x(x), _y(y)
	{
	}

	Vector Vector::norm() const
	{
		float m = mag();
		return { _x / m, _y / m };
	}

	float Vector::mag() const
	{
		return sqrt(_x * _x + _y * _y);
	}

	float Vector::mag2() const
	{
		return _x * _x + _y * _y;
	}

	float Vector::dot(const Vector& other) const
	{
		return _x * other._x + _y * other._y;
	}

	float Vector::angle(const Vector& other) const
	{
		return acosf(dot(other) / (mag() * other.mag()));
	}

	Vector Vector::operator+(const Vector& other)
	{
		return { _x + other._x, _y + other._y };
	}

	Vector Vector::operator-(const Vector& other)
	{
		return { _x - other._x, _y - other._y };
	}
	Vector Vector::operator*(const float value)
	{
		return { _x * value, _y * value };
	}
	Vector Vector::operator/(const float value)
	{
		return { _x / value, _y / value };
	}
}
