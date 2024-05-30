#pragma once
#include <memory>
#include "Atom.h"
#include "Bond.h"
#include <vector>


class Renderer
{
public:
	Renderer();

	void init();
	void render(const std::vector<std::shared_ptr<Atom>>& atoms, const std::vector<std::shared_ptr<Bond>>& bonds);
};

