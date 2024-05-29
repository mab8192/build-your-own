#include "raylib.h"
#include "Renderer.h"

Renderer::Renderer()
{
}

void Renderer::init()
{
}

void Renderer::render(const std::vector<Atom>& atoms)
{
	BeginDrawing();
	ClearBackground(BLACK);

	for (const Atom& atom : atoms) {
		DrawCircleV(atom.getPosition(), 3.0f, atom.getColor());
	}

	DrawCircle(100, 300, 10.f, BLUE);
	DrawCircle(700, 300, 10.f, BLUE);

	EndDrawing();
}

