#include "Bond.h"
#include "Config.h"
#include "raylib.h"
#include "raymath.h"
#include "Renderer.h"
#include "World.h"
#include <iostream>
#include <string>
#include <memory>

struct AppSpec {
	int windowWidth, windowHeight;
	std::string title;
	unsigned int flags;
};

class Application {
public:
	Application(const AppSpec& spec) 
		: _spec(spec), _world(World())
	{
		SetConfigFlags(_spec.flags);
		InitWindow(_spec.windowWidth, _spec.windowHeight, _spec.title.c_str());

		SetTargetFPS(60);

		for (int i = 0; i < 100; i++)
		{
			Atom::Properties props;
			props.color = RED;
			_world.spawn(Vector2{ static_cast<float>(GetRandomValue(10, _spec.windowWidth - 10)),
						static_cast<float>(GetRandomValue(10, _spec.windowHeight - 10)) },
				props); // the atom makes its own copy of props, fine to destroy here
		}
	}

	void update(float delta)
	{
		// handle input
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !pickedAtom)
		{
			std::shared_ptr<Atom> atom = _world.pickAtom(GetMousePosition(), ATOM_RADIUS);
			if (atom)
			{
				std::cout << "Picked atom " << atom->getID() << std::endl;
				pickedAtom = atom;
			}
			// atom dies here and reference count decs
		}
		else if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
		{
			pickedAtom.reset();
		}

		if (pickedAtom)
		{
			/*
			* Vector2 dir = Vector2Normalize(Vector2Subtract(GetMousePosition(), pickedAtom->getPosition()));
			* Vector2 force = Vector2Scale(dir, 2000.0f);
			* pickedAtom->applyForce(force);
			*/
			pickedAtom->setPosition(GetMousePosition());
		}

		// Update the world
		_world.tick(delta);
	}

	void run() 
	{
		float delta = 0;

		while (!WindowShouldClose()) 
		{
			float startTime = GetTime();

			update(delta);
			_renderer.render(_world.getAtoms(), _world.getBonds());

			delta = GetTime() - startTime;
		}
	}

	void close() 
	{

	}

private:
	AppSpec _spec;
	Renderer _renderer;
	World _world;

	std::shared_ptr<Atom> pickedAtom;
};

int main() {
	Application app({ 800, 600, "Emergent Worlds 2D", (int) FLAG_MSAA_4X_HINT | FLAG_WINDOW_ALWAYS_RUN });
	app.run();
	app.close();

	return 0;
}