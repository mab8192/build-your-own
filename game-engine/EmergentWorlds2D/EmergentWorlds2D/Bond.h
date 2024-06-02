#pragma once
#include "Atom.h"
#include <memory>

class Bond
{
public:
	Bond(std::shared_ptr<Atom> a1, std::shared_ptr<Atom> a2, float strength);

	std::shared_ptr<Atom> getA1() const;
	std::shared_ptr<Atom> getA2() const;

	Vector2 getForce() const;

private:
	std::shared_ptr<Atom> _a1, _a2;
	float _strength = 10.0f;
	// bond type ?? ionic, covalent, hydrogen
	// how do these types affect the force applied?
};
