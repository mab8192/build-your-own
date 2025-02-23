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
	virtual void Draw() const = 0;

	// main driver loop
	virtual void Run() final;

protected:
	bool m_isRunning = true;
	bool m_isPaused = false;
	bool m_isDebug = false;

private:
	void RunPhysics();

	// syncing between regular + physics threads
	std::atomic<bool> m_ShouldClose { false };
	std::mutex m_Mutex;
};

