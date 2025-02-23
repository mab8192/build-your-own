#include "raylib-cpp.hpp"

namespace rl = raylib;

namespace Config {
	const int WIDTH = 256;
	const int HEIGHT = 256;

	constexpr int NUM_CELLS = WIDTH * HEIGHT;
}

int IX(int x, int y) {
	return x + y * Config::WIDTH;
}

struct Fluid {
	float dt;
	float diff;
	float visc;

	float* s;
	float* density;

	float* Vx;
	float* Vy;

	float* Vx0;
	float* Vy0;

	Fluid(float dt, float diff, float visc)
		: dt(dt), diff(diff), visc(visc) 
	{
		s = new float[Config::NUM_CELLS];
		density = new float[Config::NUM_CELLS];

		Vx = new float[Config::NUM_CELLS];
		Vy = new float[Config::NUM_CELLS];

		Vx0 = new float[Config::NUM_CELLS];
		Vy0 = new float[Config::NUM_CELLS];
	}

	~Fluid() {
		delete[] s;
		delete[] density;
		delete[] Vx;
		delete[] Vy;
		delete[] Vx0;
		delete[] Vy0;
	}

	void addDensity(int x, int y, float amount) {
		int index = IX(x, y);
		density[index] += amount;
	}

	void addVelocity(int x, int y, float vx, float vy) {
		int index = IX(x, y);
		Vx[index] += vx;
		Vy[index] += vy;
	}

	void diffuse() {

	}
};

struct Application {
	rl::Window _window;

	Application() {
		// Create window
		_window.Init(Config::WIDTH, Config::HEIGHT, "Fluid Simulation", FLAG_MSAA_4X_HINT);
		_window.SetTargetFPS(120);
	}

	void run() {
		while (!WindowShouldClose()) {
			_window.BeginDrawing();
			_window.ClearBackground(BLACK);

			

			_window.DrawFPS();
			_window.EndDrawing();
		}

		_window.Close();
	}
};

int main(int argc, char** argv) {
	Application app;
	app.run();
}