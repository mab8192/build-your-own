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

	for (const std::shared_ptr<Atom>& atom : atoms) {
		DrawCircleV(atom->getPosition(), 3.0f, atom->getColor());
	}

	for (const std::shared_ptr<Bond>& bond : bonds)
	{
		DrawLineV(bond->getA1().getPosition(), bond->getA2().getPosition(), WHITE);
	}

	EndDrawing();
}
