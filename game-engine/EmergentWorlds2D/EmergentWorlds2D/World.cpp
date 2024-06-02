#include "Config.h"
#include "World.h"
#include <raymath.h>
#include <iostream>

void World::spawn(Vector2 pos, const Atom::Properties& props)
{
	_atoms.push_back(std::make_shared<Atom>(_atoms.size(), pos, props));
}

const std::vector< std::shared_ptr<Atom>>& World::getAtoms() const
{
	return _atoms;
}

const std::vector< std::shared_ptr<Bond>>& World::getBonds() const
{
	return _bonds;
}

const std::shared_ptr<Atom> World::pickAtom(Vector2 coords, float radius) const
{
	std::shared_ptr<Atom> result;
	float min = radius*2;

	for (auto atom : _atoms)
	{
		float dist = Vector2Distance(coords, atom->getPosition());
		if (dist < radius && dist < min)
		{
			min = dist;
			result = atom;
		}
	}
	// no need to delete
	return result;
}

void World::tick(float delta)
{
	// Form new bonds and break existing ones that are too far away
	for (size_t i = 0; i < _atoms.size(); i++)
	{
		auto a1 = _atoms[i];
		for (size_t j = i + 1; j < _atoms.size(); j++)
		{
			auto a2 = _atoms[j];
			float dist = Vector2Distance(a1->getPosition(), a2->getPosition());
			bool shouldBond = dist < BOND_TARGET_DISTANCE && a1->canFormBond(a2) && a2->canFormBond(a1);

			if (shouldBond)
			{
				_bonds.push_back(std::make_shared<Bond>(a1, a2, BOND_STRENGTH));
			}

			// Apply natural repelling force if atoms are too close
			if (dist < MIN_SEPARATION_DIST)
			{
				Vector2 sepForce = Vector2Scale(Vector2Normalize(Vector2Subtract(a2->getPosition(), a1->getPosition())), -SEPARATION_FORCE_STRENGTH);
				
				a1->applyForce(sepForce);
				a2->applyForce(Vector2Negate(sepForce));
			}
		}
	}

	// Apply bond forces
	for (auto bond : _bonds)
	{
		auto a1 = bond->getA1();
		auto a2 = bond->getA2();

		// force from atom 1 to atom 2
		Vector2 force12 = bond->getForce();

		a1->applyForce(force12);
		a2->applyForce(Vector2Negate(force12));
	}

	for (auto atom : _atoms)
	{
		atom->tick(delta);
	}
}
