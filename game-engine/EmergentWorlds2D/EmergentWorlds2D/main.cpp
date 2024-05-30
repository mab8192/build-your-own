#include "Bond.h"
#include "raylib.h"
#include "raymath.h"
#include "Renderer.h"
#include <string>

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

		SetTargetFPS(60);

		for (int i = 0; i < 10; i++) {
			_atoms.push_back(std::make_shared<Atom>(Vector2{ static_cast<float>(GetRandomValue(10, _spec.windowWidth - 10)),
						static_cast<float>(GetRandomValue(10, _spec.windowHeight - 10)) },
				RED));
		}
	}

	void update() {
		// handle input
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		{
			_atoms.emplace_back(std::make_shared<Atom>(GetMousePosition(), BLUE));
		}

		// Form new bonds and break existing ones that are too far away
		for (int i = 0; i < _atoms.size(); i++) {
			Atom& a1 = *_atoms[i];
			for (int j = i + 1; j < _atoms.size(); j++) {
				Atom& a2 = *_atoms[j];
				float dist = Vector2Distance(a1.getPosition(), a2.getPosition());
				// If this pair of atoms can form a bond...
				// TODO: Make sure they aren't already bonded!
				if (dist < BOND_TARGET_DISTANCE && a1.canFormBond() && a2.canFormBond()) {
					// create bond
					_bonds.emplace_back(std::make_shared<Bond>(_atoms[i], _atoms[j], 10.0f));
					a1.setColor(GREEN);
					a2.setColor(GREEN);
				}
			}
		}

		std::vector<std::shared_ptr<Bond>> remainingBonds;
		for (std::shared_ptr<Bond>& bondp : _bonds)
		{
			Bond& bond = *bondp.get();
			if (!bond.shouldBreak()) {
				// apply force
				Vector2 force = bond.getForce();
				bond.getA1().applyForce(force); 
				bond.getA2().applyForce(Vector2Negate(force)); 
				remainingBonds.push_back(bondp);
			}
		}
		_bonds = remainingBonds;

		printf("number of bonds: %d\n", _bonds.size());

		// tick atoms
		for (std::shared_ptr<Atom>& atom : _atoms) {
			atom->tick(_deltaTime);
		}
	}

	void run() {
		while (!WindowShouldClose()) {
			double startTime = GetTime();
			update();
			_renderer.render(_atoms, _bonds);
			_deltaTime = GetTime() - startTime;
		}
	}

	void close() {

	}

private:
	AppSpec _spec;
	Renderer _renderer;
	std::vector<std::shared_ptr<Atom>> _atoms;
	std::vector<std::shared_ptr<Bond>> _bonds;
	double _deltaTime;
};


int main() {
	Application app({ 800, 600, "Emergent Worlds 2D", (int) FLAG_MSAA_4X_HINT | FLAG_WINDOW_ALWAYS_RUN });
	app.init();
	app.run();
	app.close();

	return 0;
}