#pragma once

#include <atomic>
#include <mutex>

class Driver
{
public:
	Driver();
	~Driver();

	// pure virtual functions, each version must implement these
	virtual void ProcessInputs() = 0;
	virtual void FixedUpdate(double dt) = 0;
	virtual void Update(double dt) = 0;
	virtual void Draw() = 0;

	// main driver loop
	virtual void Run() final;

	void setPhysicsTimeStep(float newTimeStep);

protected:
	bool m_isRunning = true;
	bool m_isPaused = false;
	bool m_isDebug = false;

	float m_SimTimeStep = 1.0f / 60.0f; // default 60hz time step
	float m_TimeScale = 1.0f;

private:
	void RunPhysics();

	// syncing between regular + physics threads
	std::atomic<bool> m_ShouldClose { false };
	std::mutex m_Mutex;
};

