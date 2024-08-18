#include "raylib.h"
#include "raymath.h"
#include <vector>
#include <memory>
#include <algorithm>
#include <functional>

namespace Config {
	const int WIDTH = 1280;
	const int HEIGHT = 800;

	const float GRAVITY = 0;
	const float COLLISION_DAMPING = 0.7f;
	const float PARTICLE_SIZE = 2.0f;
	const float PARTICLE_SPACING = 0.0f;
	const float TARGET_DENSITY = 0.01f;
	const float PRESSURE_FORCE = 10.0f;

	float particleSmoothingRadius = 30.0f;
}

struct Particle {
	Vector2 pos{};
	Vector2 vel{};
	Vector2 acc{};

	float density = -1.0f;
	float mass = 1.0f;

	Particle() = default;

	Particle(int x, int y) {
		pos.x = x;
		pos.y = y;
	}

	void update(double dt) {
		vel.x += acc.x * dt;
		vel.y += acc.y * dt;

		pos.x += vel.x * dt;
		pos.y += vel.y * dt;
	}
};

struct World {
	std::vector<std::shared_ptr<Particle>> particles;
	float inset = 50;
	Rectangle bbox{ inset, inset, Config::WIDTH - 2*inset, Config::HEIGHT - 2*inset };

	std::shared_ptr<Particle> spawn(int x, int y) {
		std::shared_ptr<Particle> p = std::make_shared<Particle>(x, y);

		p.get()->acc.y = Config::GRAVITY; // set gravity

		particles.push_back(p);
		return p;
	}
	
	void update(double dt) {
		for (auto p : particles) {
			p.get()->update(dt);

			resolveCollisions();
		}
	}

	void resolveCollisions() {

		for (auto pref : particles) {
			Particle& p = *pref;
			
			if (p.pos.x - Config::PARTICLE_SIZE < bbox.x) {
				p.pos.x = bbox.x + Config::PARTICLE_SIZE;
				p.vel.x *= -1 * Config::COLLISION_DAMPING;
			} 
			
			if (p.pos.x + Config::PARTICLE_SIZE > bbox.x + bbox.width) {
				p.pos.x = bbox.x + bbox.width - Config::PARTICLE_SIZE;
				p.vel.x *= -1 * Config::COLLISION_DAMPING;
			}

			if (p.pos.y - Config::PARTICLE_SIZE < bbox.y) {
				p.pos.y = bbox.y + Config::PARTICLE_SIZE;
				p.vel.y *= -1 * Config::COLLISION_DAMPING;
			}
				
			if (p.pos.y + Config::PARTICLE_SIZE > bbox.y + bbox.height) {
				p.pos.y = bbox.y + bbox.height - Config::PARTICLE_SIZE;
				p.vel.y *= -1 * Config::COLLISION_DAMPING;
			}
		}
	}

	float smoothingFunc(float r, float d) {
		float volume = PI * pow(r, 8) / 4.0f;
		float v = std::max(0.0f, r * r - d * d);
		return v * v * v / volume;
	}

	float smoothingFuncDerivative(float r, float d) {
		if (d >= r) return 0;
		float f = r * r - d * d;
		float scale = -24 / (PI * pow(r, 8));
		return scale * d * f * f;
	}

	float calculateDensity(Vector2 samplePoint) {
		float density = 0;

		for (auto pref : particles) {
			float dst = Vector2Distance(pref.get()->pos, samplePoint);
			float influence = smoothingFunc(Config::particleSmoothingRadius, dst);
			density += pref.get()->mass * influence;
		}

		return density;
	}

	void calculateDensities() {
		for (auto pref : particles) {
			pref.get()->density = calculateDensity(pref.get()->pos);
		}
	}

	float calculateProperty(Vector2 samplePoint, std::function<float(const std::shared_ptr<Particle>)> propertySelector) {
		float prop = 0;

		for (auto pref : particles) {
			float dst = Vector2Distance(samplePoint, pref.get()->pos);
			float influence = smoothingFunc(Config::particleSmoothingRadius, dst);
			float density = calculateDensity(pref.get()->pos);
			prop += propertySelector(pref) * pref.get()->mass / density * influence;
		}

		return prop;
	}

	Vector2 calculatePropertyGradient(Vector2 samplePoint, std::function<float(const std::shared_ptr<Particle>)> propertySelector) {
		Vector2 propGrad{};

		for (auto pref : particles) {
			float dst = Vector2Distance(samplePoint, pref.get()->pos);
			Vector2 dir = Vector2Scale(Vector2Subtract(samplePoint, pref.get()->pos), 1/dst);
			float slope = smoothingFuncDerivative(Config::particleSmoothingRadius, dst);
			float density = pref.get()->density;
			propGrad.x += dir.x * propertySelector(pref) * pref.get()->mass / density * slope;
			propGrad.y += dir.y * propertySelector(pref) * pref.get()->mass / density * slope;
		}

		return propGrad;
	}

	float calculatePressure(float density) {
		float densityE = density - Config::TARGET_DENSITY;
		float pressure = densityE * Config::PRESSURE_FORCE;
		return pressure;
	}
};

struct Application {
	World world;

	Application() {
		// Create window
		SetConfigFlags(FLAG_MSAA_4X_HINT);
		InitWindow(Config::WIDTH, Config::HEIGHT, "Fluid Simulation");
		SetTargetFPS(120);
	}

	void createWorld(int gridSize) {
		float spacing = Config::PARTICLE_SIZE * 2 + Config::PARTICLE_SPACING;

		int startX = Config::WIDTH / 2 - (gridSize / 2 * spacing);
		int endX = Config::WIDTH / 2 + (gridSize / 2 * spacing);
		int startY = Config::HEIGHT / 2 - (gridSize / 2 * spacing);
		int endY = Config::HEIGHT / 2 + (gridSize / 2 * spacing);

		for (int x = startX; x < endX; x += spacing) {
			for (int y = startY; y < endY; y += spacing) {
				auto particle = world.spawn(x, y);
			}
		}
	}

	void createRandomWorld(int numParticles) {
		for (int i = 0; i < numParticles; i++) {
			world.spawn(GetRandomValue(world.bbox.x + Config::PARTICLE_SIZE, world.bbox.x + world.bbox.width - Config::PARTICLE_SIZE),
				GetRandomValue(world.bbox.y + Config::PARTICLE_SIZE, world.bbox.y + world.bbox.height - Config::PARTICLE_SIZE));
		}
	}

	void run() {
		world.calculateDensities();

		float maxDensity = -1;
		for (auto pref : world.particles) {
			maxDensity = std::max(maxDensity, pref.get()->density);
		}

		while (!WindowShouldClose()) {
			BeginDrawing();
			ClearBackground(BLACK);

			// Input
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
				world.spawn(GetMouseX(), GetMouseY());
			}

			Config::particleSmoothingRadius += GetMouseWheelMove() * 3;
			Config::particleSmoothingRadius = Clamp(Config::particleSmoothingRadius, 1.0f, 200.0f);

			// Update
			world.update(GetFrameTime());
			
			// Draw
			
			// Draw properties
			int stepX = Config::WIDTH / 100;
			int stepY = Config::HEIGHT / 100;
			for (int i = 0; i < Config::WIDTH; i += stepX) {
				for (int j = 0; j < Config::HEIGHT; j += stepY) {
					float density = world.calculateDensity({ i + stepX/2.0f, j + stepY/2.0f });

					unsigned char vis = std::min(density * 400000, 255.0f);

					DrawRectangle(i, j, stepX, stepY, { 0, 0, vis, 128 });
				}
			}

			for (auto p : world.particles) {
				DrawCircleV(p.get()->pos, Config::PARTICLE_SIZE, WHITE);
			}

			// Draw boundary
			DrawRectangleLines(world.bbox.x, world.bbox.y, world.bbox.width, world.bbox.height, GREEN);

			// Vector2 mousePos = GetMousePosition();
			// Vector2 propGrad = world.calculatePropertyGradient(mousePos, [](std::shared_ptr<Particle> p) { return p.get()->mass; });
			// DrawText(TextFormat("Property grad at mouse: %f, %f", propGrad.x, propGrad.y), 10, 10, 24, WHITE);
			// DrawCircleLinesV(mousePos, Config::particleSmoothingRadius, WHITE);

			EndDrawing();
		}

		CloseWindow();
	}
};

int main(int argc, char** argv) {
	Application app;
	// app.createWorld(10);

	app.createRandomWorld(500);
	app.run();
}