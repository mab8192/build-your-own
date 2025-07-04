#include "Simulation.h"

#include "Body.h"
#include "Camera.h"
#include "Collision.h"
#include "Common.h"
#include "Random.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "rlgl.h"

#include <iostream>
#include <thread>
#include <vector>

Simulation::Simulation()
{
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI | FLAG_MSAA_4X_HINT);
	InitWindow(WINDOW_SIZE.x, WINDOW_SIZE.y, "N-Body Simulation");

	// Set up camera
	m_Camera = { 0 };
	m_Camera.position = Vector3{ 100, 500, 100 };		// Camera position
	m_Camera.target = Vector3{ 0.0f, 0.0f, 0.0f };      // Camera looking at point
	m_Camera.up = Vector3{ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
	m_Camera.fovy = 45.0f;                              // Camera field-of-view Y

	// initialize star field
	Random& rng = Random::getInstance();
	for (int i = 0; i < 1000; i++)
	{
		m_StarField.push_back({rng.getFloat(-1000, 1000), rng.getFloat(-1000, 1000), rng.getFloat(-1000, 1000) });
	}

	// initialize bodies
	Body sun = { {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, 5000000, 20 };
	sun.color = YELLOW;
	m_Bodies.push_back(sun);

	float distance = 100.0;
	float v = std::sqrt(G * sun.mass / distance);
	
	Body p1 = Body::Generate({ distance, 0, 0 }, {0, 0, -v});
	m_Bodies.push_back(p1);

	distance = 300.0;
	v = std::sqrt(G * sun.mass / distance);

	Body p2 = Body::Generate({ distance, 0, 0 }, { 0, 0, -v });
	m_Bodies.push_back(p2);
}

Simulation::~Simulation() 
{
	CloseWindow();
}

void Simulation::RunPhysics()
{
	double timeAccumulator = 0.0;
	double currentTime = GetTime();

    while (!m_ShouldClose)
    {
        double newTime = GetTime();
        double dt = newTime - currentTime;
        currentTime = newTime;

        if (!m_isPaused)
        {
            timeAccumulator += dt;
            while (timeAccumulator >= m_SimTimeStep)
            {
                std::lock_guard<std::mutex> lock(m_Mutex);
                FixedUpdate(m_SimTimeStep * m_TimeScale);
                timeAccumulator -= m_SimTimeStep;
            }
        }
        else
        {
            // if paused, just sleep for a bit to avoid busy waiting
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }
}

void Simulation::Run()
{
	// run physics on a separate thread
	std::thread physicsThread(&Simulation::RunPhysics, this);

	while (!WindowShouldClose())
	{
		{
			std::lock_guard<std::mutex> lock(m_Mutex);
			ProcessInputs();
			Update(GetFrameTime());
		}
		Draw();
	}

	m_ShouldClose = true;
	physicsThread.join();
}

void Simulation::setPhysicsTimeStep(float newTimeStep)
{
    m_SimTimeStep = newTimeStep;
}

void Simulation::ProcessInputs() 
{
	if (IsKeyPressed(KEY_SPACE))
	{
		m_isPaused = !m_isPaused;
	}

	if (IsKeyPressed(KEY_P))
	{
		m_isDebug = !m_isDebug;
	}

	if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE))
	{
		FPCamera(m_Camera);
	}

	if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON))
	{
		OrbitCamera(m_Camera);
	}
}

void Simulation::FixedUpdate(double dt) 
{
	delete m_Octree;
	m_Octree = Octree::buildOctree(m_Bodies, m_UniverseSize);

	// Calculate accelerations with octree optimization
	for (Body& b : m_Bodies) 
	{
		b.acc = { 0, 0, 0 };

		// For gravity, we might still need to check all bodies
		// But for distant bodies, we could use center-of-mass approximation
		for (Body& other : m_Bodies) 
		{
			if (&b == &other) continue;
			Vector3 r = Vector3Subtract(other.pos, b.pos);
			float distance = Vector3Length(r);
			float force = G * b.mass * other.mass / (distance * distance);
			Vector3 dir = Vector3Normalize(r);
			b.acc = Vector3Add(b.acc, Vector3Scale(dir, force / b.mass));
		}
	}

	// Update positions and velocities
	for (Body& b : m_Bodies) 
	{
		b.vel = Vector3Add(b.vel, Vector3Scale(b.acc, dt));
		b.pos = Vector3Add(b.pos, Vector3Scale(b.vel, dt));

		if (Vector3Distance(b.pos, { 0, 0, 0 }) > m_UniverseSize) {
			b.deleted = true;
		}
	}

	// Resolve collisions using octree for efficient neighbor finding
	const int COLLISION_ITERATIONS = 2;
	for (int iter = 0; iter < COLLISION_ITERATIONS; iter++) 
	{
		for (Body& b : m_Bodies) {
			if (b.deleted) continue;

			std::vector<Body*> potentialCollisions;
			m_Octree->queryPotentialCollisions(&b, b.radius * 2, potentialCollisions);

			for (Body* other : potentialCollisions) 
			{
				if (&b != other && !b.deleted && !other->deleted) 
				{
					resolveCollisionCombine(b, *other);
				}
			}
		}
	}

	// Clean up deleted bodies at the end
	std::erase_if(m_Bodies, [](const Body& body) { return body.deleted; });
}

void Simulation::Update(double dt) 
{

}

void Simulation::Draw()
{
	BeginDrawing();
	ClearBackground(BLACK);

	BeginMode3D(m_Camera);
	SetCustomProjectionMatrix(m_Camera, 10000.0f); // Set far clipping plane to 10000.0f

	//for (const Vector3& star : m_StarField)
	//{
	//	// TODO: Not working
	//	Vector2 screenPos = GetWorldToScreen(star, m_Camera);
	//	DrawCircleV(screenPos, 2, WHITE);
	//}

	for (const Body& b : m_Bodies)
	{
		DrawSphere(b.pos, b.radius, b.color);
		if (m_isDebug)
		{
			DrawLine3D(b.pos, Vector3Add(b.pos, b.acc), RED);
			DrawLine3D(b.pos, Vector3Add(b.pos, b.vel), GREEN);
		}
	}

	EndMode3D();

	// UI elements
	DrawFPS(10, 10);

	if (m_isPaused)
	{
		DrawText("PAUSED", 10, 30, 24, RED);
	}

	if (m_isDebug)
	{
		DrawText("DEBUG", 10, 50, 24, RED);
	}

	DrawText(TextFormat("Number of bodies: %i", m_Bodies.size()), 100, 10, 20, DARKGREEN);

	DrawText(TextFormat("Time scale: %f", m_TimeScale), 100, 130, 16, WHITE);
	GuiSliderBar({ 100, 150, 120, 10 }, "", "", &m_TimeScale, 0.5f, 4.0f);

	EndDrawing();
}

// end of Simulation.cpp