#pragma once
#include "raylib.h"

class Atom
{
public:
	Atom(Vector2 pos, Color col);

	const Vector2& getPosition() const;
	const Vector2& getVelocity() const;
	const Vector2& getAcceleration() const;
	const Color& getColor() const;
	const int getMaxBonds() const;

	bool canFormBond() const;
	void addBond();
	void breakBond();

	void setColor(Color color);

	void applyForce(const Vector2& force);

	void tick(double delta);

private:
	Vector2 _pos, _vel, _acc;
	float _mass, _temp;
	Color _color;
	int _maxBonds = 3;
	int _numBonds = 0;
};
