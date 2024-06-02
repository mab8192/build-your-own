#include "Config.h"
#include "raylib.h"
#include "Renderer.h"

Renderer::Renderer()
{
}

void Renderer::init()
{
}

void Renderer::render(const std::vector<std::shared_ptr<Atom>>& atoms, const std::vector<std::shared_ptr<Bond>>& bonds)
{
	BeginDrawing();
	ClearBackground(BLACK);

	for (auto atom : atoms) {
		DrawCircleV(atom->getPosition(), ATOM_RADIUS, atom->getProps().color);
	}

	for (auto bond : bonds)
	{
		DrawLineV(bond->getA1()->getPosition(), bond->getA2()->getPosition(), WHITE);
	}

	DrawFPS(10, 10);

	EndDrawing();
}
