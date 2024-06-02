#include "Bond.h"
#include "Config.h"
#include <raymath.h>
#include <iostream>


Bond::Bond(std::shared_ptr<Atom> a1, std::shared_ptr<Atom> a2, float strength)
	: _a1(a1), _a2(a2), _strength(strength)
{
	_a1->addBond(_a2);
	_a2->addBond(_a1);
}

std::shared_ptr<Atom> Bond::getA1() const
{
	return _a1;
}

std::shared_ptr<Atom> Bond::getA2() const
{
	return _a2;
}

Vector2 Bond::getForce() const
{
	Vector2 a1Pos = _a1->getPosition();
	Vector2 a2Pos = _a2->getPosition();
	float offset = Vector2Distance(a1Pos, a2Pos) - BOND_TARGET_DISTANCE;
	Vector2 force = Vector2Scale(Vector2Normalize(Vector2Subtract(a2Pos, a1Pos)), _strength * offset);

	// ignore tiny forces for stability reasons
	if (Vector2Length(force) < BOND_MIN_FORCE) return { 0, 0 };
	return force;
}
