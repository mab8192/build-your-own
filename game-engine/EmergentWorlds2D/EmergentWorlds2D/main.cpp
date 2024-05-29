#include <string>
#include "raylib.h"
#include "Renderer.h"
#include "raymath.h"

struct AppSpec {
	int windowWidth, windowHeight;
	std::string title;
	unsigned int flags;
};

class Application {
public:
	Application(const AppSpec& spec) 
		: _spec(spec), _deltaTime(0) {

	}

	void init() {
		SetConfigFlags(_spec.flags);
		InitWindow(_spec.windowWidth, _spec.windowHeight, _spec.title.c_str());

		for (int i = 0; i < 100; i++) {
			_atoms.emplace_back(
				Vector2{static_cast<float>(GetRandomValue(10, _spec.windowWidth - 10)), 
						static_cast<float>(GetRandomValue(10, _spec.windowHeight - 10))}, 
				RED);
		}
	}

	void update() {
		// handle input
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			Vector2 mousePos = GetMousePosition();
			_atoms.emplace_back(mousePos, RED);
		}

		// apply forces
		for (Atom& atom : _atoms) {
			Vector2 bh1 = Vector2{ 100, 300 };
			float dist1 = Vector2DistanceSqr(bh1, atom.getPosition());
			Vector2 force1 = Vector2Scale(Vector2Subtract(atom.getPosition(), bh1), -400.0f/dist1);
			atom.applyForce(force1);

			Vector2 bh2 = Vector2{ 700, 300 };
			float dist2 = Vector2DistanceSqr(bh2, atom.getPosition());
			Vector2 force2 = Vector2Scale(Vector2Subtract(atom.getPosition(), bh2), -400.0f / dist2);
			atom.applyForce(force2);
		}

		// tick atoms
		for (Atom& atom : _atoms) {
			atom.tick(_deltaTime);
		}
	}

	void run() {
		while (!WindowShouldClose()) {
			double startTime = GetTime();
			update();
			_renderer.render(_atoms);
			_deltaTime = GetTime() - startTime;
		}
	}

	void close() {

	}

private:
	AppSpec _spec;
	Renderer _renderer;
	std::vector<Atom> _atoms;
	double _deltaTime;
};


int main() {
	Application app({ 800, 600, "Emergent Worlds 2D", (int) FLAG_MSAA_4X_HINT | FLAG_WINDOW_ALWAYS_RUN });
	app.init();
	app.run();
	app.close();

	return 0;
}