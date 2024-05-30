#include "Bond.h"
#include <raymath.h>
#include <iostream>

Bond::Bond(std::shared_ptr<Atom> a1, std::shared_ptr<Atom> a2, float strength)
	: _a1(a1), _a2(a2), _strength(strength)
{
	a1->addBond();
	a2->addBond();
}

Vector2 Bond::getForce() const
{
	float offset = Vector2Distance(_a1->getPosition(), _a2->getPosition()) - BOND_TARGET_DISTANCE;
	Vector2 force = Vector2Scale(Vector2Normalize(Vector2Subtract(_a2->getPosition(), _a1->getPosition())), _strength * offset);

	// ignore tiny forces
	if (Vector2Length(force) < BOND_MIN_FORCE || _broken) return { 0, 0 };
	return force;
}

Atom& Bond::getA1() const
{
	return *_a1;
}

Atom& Bond::getA2() const
{
	return *_a2;
}

bool Bond::shouldBreak() const
{
	if (_broken) return true;

	if (Vector2Distance(_a1->getPosition(), _a2->getPosition()) >= BOND_BREAK_DISTANCE)
	{
		_broken = true;
		_a1->breakBond();
		_a2->breakBond();
		return true;
	}

	return false;
}
