#pragma once
#include "Atom.h"
#include <memory>

#define BOND_TARGET_DISTANCE 20.0f
#define BOND_BREAK_DISTANCE 40.0f
#define BOND_MIN_FORCE 10.0f

class Bond
{
public:
	Bond(std::shared_ptr<Atom> a1, std::shared_ptr<Atom> a2, float strength);

	Vector2 getForce() const;

	Atom& getA1() const;
	Atom& getA2() const;

	bool shouldBreak() const;

private:
	std::shared_ptr<Atom> _a1;
	std::shared_ptr<Atom> _a2;
	float _strength;
	mutable bool _broken = false;
};

