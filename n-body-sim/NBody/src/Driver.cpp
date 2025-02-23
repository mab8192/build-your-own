#include "Driver.h"
#include "common.h"
#include <thread>

Driver::Driver()
{
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI | FLAG_MSAA_4X_HINT);
	InitWindow(WINDOW_SIZE.x, WINDOW_SIZE.y, "N-Body Simulation");
}

Driver::~Driver() {
	CloseWindow();
}

void Driver::RunPhysics()
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

void Driver::Run()
{
	std::thread physicsThread(&Driver::RunPhysics, this);

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

void Driver::setPhysicsTimeStep(float newTimeStep)
{
    m_SimTimeStep = newTimeStep;
}
// end of Driver.cpp