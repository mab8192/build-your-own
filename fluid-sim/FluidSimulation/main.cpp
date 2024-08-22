#include "raylib-cpp.hpp"
#include <vector>
#include <memory>
#include <algorithm>
#include <functional>

namespace Config {
	const int WIDTH = 1500;
	const int HEIGHT = 1000;

	const float GRAVITY = 0.0f;
	const float COLLISION_DAMPING = 0.95f;
	const float VELOCITY_DAMPING = 0.98f;
	const float PARTICLE_SIZE = 6.0f;
	const float PARTICLE_SPACING = 0.0f;
	const float PRESSURE_FORCE = 10000.0f;

	float targetDensity = 1.0f;
	float particleSmoothingRadius = 75.0f;
}

struct Particle {
	raylib::Vector2 pos{};
	raylib::Vector2 vel{};
	raylib::Vector2 acc{};

	float density = -1.0f;
	float mass = 1.0f;

	Particle() = default;

	Particle(int x, int y) {
		pos.x = x;
		pos.y = y;
	}

	void applyForce(const raylib::Vector2& force) {
		acc += force / density; // density, not mass here. working with small volumes of fluid, not an actual particle
	}

	void update(double dt) {
		vel += acc * dt;

		// cap velocity
		vel = Vector2Clamp(vel, raylib::Vector2{ -30, -30 }, raylib::Vector2{ 30, 30 });

		pos += vel * dt;

		vel *= Config::VELOCITY_DAMPING;

		acc = raylib::Vector2::Zero();
	}
};

struct World {
	std::vector<std::shared_ptr<Particle>> particles;
	float inset = 100;
	raylib::Rectangle bbox = raylib::Rectangle(inset, inset, Config::WIDTH - 2 * inset, Config::HEIGHT - 2 * inset);

	std::shared_ptr<Particle> spawn(int x, int y) {
		std::shared_ptr<Particle> p = std::make_shared<Particle>(x, y);

		p.get()->acc.y = Config::GRAVITY; // set gravity

		particles.push_back(p);
		return p;
	}
	
	void update(double dt) {
		calculateDensities(dt);

		// apply forces
		for (auto p : particles) {
			// apply gravity
			p.get()->applyForce({ 0, Config::GRAVITY });

			// apply pressure force
			p.get()->applyForce(calculateParticlePressure(p));
		}

		// Update positions and resolve collisions
		for (auto pref : particles) {
			pref.get()->update(dt);
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
		if (d >= r) return 0;
		float volume = (PI * pow(r, 4)) / 6;
		return (r - d) * (r - d) / volume;
	}

	float smoothingFuncDerivative(float r, float d) {
		if (d >= r) return 0;
		float scale = 12 / (pow(r, 4) * PI);
		return (d - r) * scale;
	}

	float calculateDensity(raylib::Vector2 samplePoint) {
		float density = 0;

		for (auto pref : particles) {
			float dst = Vector2Distance(pref.get()->pos, samplePoint);
			float influence = smoothingFunc(Config::particleSmoothingRadius, dst);
			density += pref.get()->mass * influence;
		}

		return density;
	}

	void calculateDensities(double dt) {
		for (auto pref : particles) {
			// Use future predicted position when computing density
			pref.get()->density = calculateDensity(pref.get()->pos + pref.get()->vel * dt);
		}
	}

	float calculateProperty(raylib::Vector2 samplePoint, std::function<float(const std::shared_ptr<Particle>)> propertySelector) {
		float prop = 0;

		for (auto pref : particles) {
			float dst = Vector2Distance(samplePoint, pref.get()->pos);
			float influence = smoothingFunc(Config::particleSmoothingRadius, dst);
			float density = calculateDensity(pref.get()->pos);
			prop += propertySelector(pref) * pref.get()->mass / density * influence;
		}

		return prop;
	}

	raylib::Vector2 calculatePropertyGradient(raylib::Vector2 samplePoint, std::function<float(const std::shared_ptr<Particle>)> propertySelector) {
		raylib::Vector2 propGrad{};

		for (auto pref : particles) {
			float dst = Vector2Distance(samplePoint, pref.get()->pos);
			raylib::Vector2 dir = (samplePoint - pref.get()->pos) / dst;
			float slope = smoothingFuncDerivative(Config::particleSmoothingRadius, dst);
			float density = pref.get()->density;
			propGrad += dir * propertySelector(pref) * pref.get()->mass / density * slope;
		}

		return propGrad;
	}

	float calculatePressure(float density) {
		float densityE = density - Config::targetDensity;
		float pressure = densityE * Config::PRESSURE_FORCE;
		return pressure;
	}

	float calculateSharedPressure(float densityA, float densityB) {
		return (calculatePressure(densityA) + calculatePressure(densityB)) / 2.0f;
	}

	raylib::Vector2 calculatePressureForce(raylib::Vector2 samplePoint) {
		raylib::Vector2 force;

		for (auto pref : particles) {
			if (pref.get()->pos == samplePoint) continue;

			raylib::Vector2 offset = samplePoint - pref.get()->pos;
			float dst = offset.Length();
			raylib::Vector2 dir = dst == 0 ? raylib::Vector2{ 1, 0 } : offset.Normalize();
			float slope = smoothingFuncDerivative(Config::particleSmoothingRadius, dst);
			float density = pref.get()->density;
			float sharedPressure = calculateSharedPressure(density, calculateDensity(samplePoint));
			force += -dir * sharedPressure * pref.get()->mass / density * slope;
		}

		return force;
	}

	raylib::Vector2 calculateParticlePressure(std::shared_ptr<Particle> particle) {
		raylib::Vector2 force;

		for (auto pref : particles) {
			if (pref == particle) continue;

			raylib::Vector2 offset = particle.get()->pos - pref.get()->pos;
			float dst = offset.Length();
			raylib::Vector2 dir = dst == 0 ? raylib::Vector2{ 1, 0 } : offset.Normalize();
			float slope = smoothingFuncDerivative(Config::particleSmoothingRadius, dst);
			float density = pref.get()->density;
			float sharedPressure = calculateSharedPressure(particle.get()->density, density);
			force += -dir * sharedPressure * pref.get()->mass / density * slope;
		}

		return force;
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
		
		Config::targetDensity = world.particles.size() / (world.bbox.width * world.bbox.height);
		printf("Set target density to %f", Config::targetDensity);

		while (!WindowShouldClose()) {
			BeginDrawing();
			ClearBackground(BLACK);

			// Input
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
				world.spawn(raylib::Mouse::GetX(), raylib::Mouse::GetY());
			}

			float wheelMove = raylib::Mouse::GetWheelMove();
			if (wheelMove != 0.0f) {
				Config::targetDensity += Config::targetDensity * wheelMove * 0.1f;
				Config::targetDensity = Clamp(Config::targetDensity, 0.00001f, 10.0f);
			}

			// Update
			world.update(GetFrameTime());
			
			// Draw
			
			// Draw properties
			//float stepX = Config::WIDTH / 100.0f;
			//float stepY = Config::HEIGHT / 100.0f;
			//for (float i = world.bbox.x; i < world.bbox.x + world.bbox.width; i += stepX) {
			//	for (float j = world.bbox.y; j < world.bbox.y + world.bbox.height; j += stepY) {
			//		float density = world.calculateDensity({ i + stepX/2.0f, j + stepY/2.0f });
			//		float pressure = world.calculatePressure(density);

			//		if (pressure < 0) {
			//			unsigned char vis = std::min(-pressure * 1000, 255.0f);
			//			DrawRectangle(i, j, stepX, stepY, { 0, 0, vis, 128 });
			//		}
			//		else {
			//			unsigned char vis = std::min(pressure * 1000, 255.0f);
			//			DrawRectangle(i, j, stepX, stepY, { vis, 0, 0, 128 });
			//		}
			//	}
			//}

			for (auto p : world.particles) {
				DrawCircleV(p.get()->pos, Config::PARTICLE_SIZE, WHITE);
			}

			// Draw boundary
			world.bbox.DrawLines(GREEN);

			Vector2 mousePos = GetMousePosition();
			float density = world.calculateDensity(mousePos);
			float pressure = world.calculatePressure(density);
			DrawText(TextFormat("Target Density: %f\tDensity: %f\tPressure: %f", Config::targetDensity, density, pressure), 10, 10, 24, WHITE);

			EndDrawing();
		}

		CloseWindow();
	}
};

int main(int argc, char** argv) {
	Application app;
	// app.createWorld(10);

	app.createRandomWorld(300);
	app.run();
}