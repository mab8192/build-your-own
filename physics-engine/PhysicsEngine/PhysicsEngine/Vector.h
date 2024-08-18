#pragma once

namespace Ark {
	class Vector
	{
	public:
		Vector();
		Vector(float val);
		Vector(float x, float y);

		Vector norm() const;
		float mag() const;
		float mag2() const;

		float dot(const Vector& other) const;

		/* Returns the angle (in radians) between this vector and other. */
		float angle(const Vector& other) const;

		Vector operator+(const Vector& other);
		Vector operator-(const Vector& other);
		Vector operator*(const float value);
		Vector operator/(const float value);

	private:
		float _x, _y;
	};
}
