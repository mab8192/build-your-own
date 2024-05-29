#pragma once
#include <memory>
#include "Atom.h"
#include <vector>


class Renderer
{
public:
	Renderer();

	void init();
	void render(const std::vector<Atom>& atoms);
};

