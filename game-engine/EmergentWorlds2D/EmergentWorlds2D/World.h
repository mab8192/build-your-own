#pragma once
#include "Atom.h"
#include "Bond.h"
#include <vector>
#include <memory>

class World
{
public:
	World() = default;

	void spawn(Vector2 pos, const Atom::Properties& props);

	const std::vector<std::shared_ptr<Atom>>& getAtoms() const;
	const std::vector< std::shared_ptr<Bond>>& getBonds() const;

	const std::shared_ptr<Atom> pickAtom(Vector2 coords, float radius) const;

	void tick(float delta);
private:
	std::vector<std::shared_ptr<Atom>> _atoms; // data structure to map atom id -> atom (array for now, change later to use uuids to support removal?)
	std::vector< std::shared_ptr<Bond>> _bonds;
};
