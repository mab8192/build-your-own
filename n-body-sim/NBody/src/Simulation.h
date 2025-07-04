#pragma once

#include "Body.h"
#include "raylib.h"
#include "Octree.h"

#include <atomic>
#include <mutex>
#include <vector>

class Simulation
{
public:
	Simulation();
	~Simulation();

	virtual void ProcessInputs();
	virtual void FixedUpdate(double dt);
	virtual void Update(double dt);
	virtual void Draw();

	// main driver loop
	virtual void Run() final;

	void setPhysicsTimeStep(float newTimeStep);

private:
	void RunPhysics();

	bool m_isRunning = true;
	bool m_isDebug = false;

	float m_SimTimeStep = 1.0f / 60.0f; // default 60hz time step
	float m_TimeScale = 0.1f;

	// syncing between regular + physics threads
	std::atomic<bool> m_ShouldClose { false };
	std::atomic<bool> m_isPaused = { false  } ;
	std::mutex m_Mutex;

	std::vector<Body> m_Bodies;
	Camera3D m_Camera;

	Octree* m_Octree = nullptr;
	float m_UniverseSize = 10000.0f;

	std::vector<Vector3> m_StarField;
};

