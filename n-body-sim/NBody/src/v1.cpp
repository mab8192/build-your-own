#include "common.h"

#include <vector>
#include <string>

const Vector2 windowSize = { 1600, 1200 };
const float simTimeStep = 0.01;
const int N = 100;

double alpha = 0.0; // used for interpolated positions when rendering

Vector2 dragStart;
bool mouseDragging = false;

struct Body
{
	Vector2 p;
	Vector2 v;
	Vector2 a;
	float m = 1.0f;
	float r = 1.0f;

	Vector2 ip; // interpolated position used for rendering
};

void resolveCollision(Body& b1, Body& b2)
{
	float distance = Vector2Distance(b1.p, b2.p);
	Vector2 r = Vector2Subtract(b2.p, b1.p);

	if (distance <= b1.r + b2.r)
	{
		Vector2 norm = Vector2Normalize(r);
		Vector2 rv = Vector2Subtract(b2.v, b1.v);
		float normVel = Vector2DotProduct(rv, norm);

		if (normVel > 0) return; // velocities are separating

		// calculate impulse
		float e = 0.5f; // elasticity [0, 1], controls how much energy is lost per collision
		float j = -(1 + e) * normVel / (1 / b1.m + 1 / b2.m);

		Vector2 impulse = Vector2Scale(norm, j);
		b1.v = Vector2Subtract(b1.v, Vector2Scale(impulse, 1 / b1.m));
		b2.v = Vector2Add(b2.v, Vector2Scale(impulse, 1 / b2.m));

		// small positional correction to prevent possible overlap
		float p = 0.2f;
		float overlap = (b1.r + b2.r - distance) / distance;
		Vector2 correction = Vector2Scale(norm, overlap * (1 / (1 / b1.m + 1 / b2.m)));

		b1.p = Vector2Subtract(b1.p, Vector2Scale(correction, 1 / b1.m));
		b2.p = Vector2Add(b2.p, Vector2Scale(correction, 1 / b2.m));
	}
}

void processInputs(std::vector<Body>& bodies) {
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
	{
		dragStart = GetMousePosition();
		mouseDragging = true;
	}
	else if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
	{
		// spawn a new body
		Body b = {
			dragStart,
			Vector2Subtract(GetMousePosition(), dragStart),
			{0, 0},
			4.0f,
			2.0f
		};

		bodies.push_back(b);
		mouseDragging = false;
	}
}

void update(float dt, std::vector<Body>& bodies) {
	for (int i = 0; i < bodies.size(); i++)
	{
		Body& b1 = bodies[i];
		for (int j = 0; j < bodies.size(); j++)
		{
			if (i == j) continue;

			Body& b2 = bodies[j];

			float f = G * b1.m * b2.m / Vector2DistanceSqr(b1.p, b2.p);
			Vector2 dir1 = Vector2Normalize(Vector2Subtract(b2.p, b1.p));
			b1.a = Vector2Add(b1.a, Vector2Scale(dir1, f / b1.m));
			b2.a = Vector2Add(b2.a, Vector2Scale(dir1, -f / b2.m));

			// Check collision
			resolveCollision(b1, b2);
		}
	}

	// Update positions and velocities
	for (Body& b : bodies)
	{
		b.a = Vector2ClampValue(b.a, -100, 100); // clamp acceleration values
		b.v = Vector2Add(b.v, Vector2Scale(b.a, dt));

		Vector2 oldPos = b.p;
		Vector2 newPos = Vector2Add(b.p, Vector2Scale(b.v, dt));
		b.p = newPos;
		b.ip = Vector2Add(Vector2Scale(oldPos, 1.0 - alpha), Vector2Scale(newPos, alpha)); // interpolate position for rendering

		// window collisions
		if (b.p.x - b.r < 0)
		{
			b.p.x = b.r;
			b.v.x *= -1;
		}
		else if (b.p.x + b.r > windowSize.x)
		{
			b.p.x = windowSize.x - b.r;
			b.v.x *= -1;
		}

		if (b.p.y - b.r < 0)
		{
			b.p.y = b.r;
			b.v.y *= -1;
		}
		else if (b.p.y + b.r > windowSize.y)
		{
			b.p.y = windowSize.y - b.r;
			b.v.y *= -1;
		}

		// reset acceleration for next frame
		b.a = { };
	}
}

void draw(const std::vector<Body>& bodies) {
	BeginDrawing();
	ClearBackground(BLACK);

	// Draw
	for (const Body& b : bodies)
	{
		DrawCircleV(b.ip, b.r, WHITE);
	}

	if (mouseDragging)
	{
		// draw a line from dragStart to current mouse position
		DrawLineV(dragStart, GetMousePosition(), GREEN);
	}

	DrawFPS(10, 10);
	DrawText(("Particles: " + std::to_string(bodies.size())).c_str(), 10, 30, 20, GREEN);
	EndDrawing();
}

void v1() {
	// Tell the window to use vysnc, work on high DPI displays, and use multisample antialiasing
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI | FLAG_MSAA_4X_HINT);

	// Create the window and OpenGL context
	InitWindow(windowSize.x, windowSize.y, "Barnes Hut N-Body Simulation");

	std::vector<Body> bodies;

	// Add central mass
	bodies.push_back({
		{ windowSize.x / 2, windowSize.y / 2},
		{ },
		{ },
		1000000.0f,
		20.0f
		});

	for (int i = 0; i < N; i++)
	{
		Body b = {
			{GetRandomValue(0, windowSize.x), GetRandomValue(0, windowSize.y)},
			{GetRandomValue(-50, 50), GetRandomValue(-50, 50)},
			{0, 0},
			10.0f,
			2.0f
		};

		// copied into vector
		bodies.push_back(b);
	}

	double timeAccumulator = 0.0;
	double currentTime = GetTime();

	// game loop
	while (!WindowShouldClose())
	{
		double newTime = GetTime();
		double dt = newTime - currentTime;
		currentTime = newTime;

		timeAccumulator += dt;

		processInputs(bodies);
		while (timeAccumulator >= simTimeStep)
		{
			update(simTimeStep, bodies);
			timeAccumulator -= simTimeStep;
		}
		alpha = timeAccumulator / simTimeStep; // used for interpolated positions
		draw(bodies);
	}

	CloseWindow();
}
