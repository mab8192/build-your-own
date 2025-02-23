#include "Driver.h"
#include "common.h"
#include "rlgl.h"

#include <iostream>
#include <vector>

const float mouseSensitivity = 0.005f;

const bool CAMERA_ORBIT_MODE = true;

struct Body
{
	Vector3 pos;
	Vector3 vel;
	Vector3 acc;
	double mass = 5000;
	double radius = 2;
	Color color = WHITE;
	bool deleted = false;
};

void SetCustomProjectionMatrix(Camera camera, float far) {
	float aspect = (float)GetScreenWidth() / (float)GetScreenHeight();
	float fovy = camera.fovy;
	float near = RL_CULL_DISTANCE_NEAR;

	float top = tanf(fovy * 0.5f) * near;
	float bottom = -top;
	float right = aspect * top;
	float left = -right;

	rlMatrixMode(RL_PROJECTION);
	rlLoadIdentity();
	rlFrustum(left, right, bottom, top, near, far);
	rlMatrixMode(RL_MODELVIEW);
}

void resolveCollision(Body& b1, Body& b2)
{
	float distance = Vector3Distance(b1.pos, b2.pos);
	Vector3 r = Vector3Subtract(b2.pos, b1.pos);
	float minDist = b1.radius + b2.radius;

	if (distance <= minDist)
	{
		Vector3 norm = Vector3Normalize(r);
		Vector3 rv = Vector3Subtract(b2.vel, b1.vel);
		float normVel = Vector3DotProduct(rv, norm);

		if (normVel < 0)
		{
			// calculate impulse
			float e = 0.5f; // elasticity [0, 1], controls how much energy is lost per collision
			float j = -(1 + e) * normVel / (1 / b1.mass + 1 / b2.mass);

			Vector3 impulse = Vector3Scale(norm, j);
			b1.vel = Vector3Subtract(b1.vel, Vector3Scale(impulse, 1 / b1.mass));
			b2.vel = Vector3Add(b2.vel, Vector3Scale(impulse, 1 / b2.mass));

		}

		// Separate the bodies immediately
		float overlap = minDist - distance;
		float totalMass = b1.mass + b2.mass;
		float ratio1 = b1.mass / totalMass;
		float ratio2 = b2.mass / totalMass;

		Vector3 separation = Vector3Scale(norm, overlap);
		b1.pos = Vector3Subtract(b1.pos, Vector3Scale(separation, ratio2));
		b2.pos = Vector3Add(b2.pos, Vector3Scale(separation, ratio1));
	}
}

class Version2 : public Driver
{
public:
	Version2() : Driver() 
	{
		DisableCursor();

		// Set up camera
		m_Camera = { 0 };
		m_Camera.position = Vector3{ 100, 500, 100 };		// Camera position
		m_Camera.target = Vector3{ 0.0f, 0.0f, 0.0f };      // Camera looking at point
		m_Camera.up = Vector3{ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
		m_Camera.fovy = 45.0f;                              // Camera field-of-view Y

		// initialize bodies
		Body sun = { {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, 5000000, 20 };
		sun.color = YELLOW;
		m_Bodies.push_back(sun);
		for (int i = 0; i < 100; i++)
		{
			Body b = {
				{GetRandomValue(-1000, 1000), 0, GetRandomValue(-1000, 1000)},
				{GetRandomValue(-50, 50), GetRandomValue(-5, 5), GetRandomValue(-50, 50)},
				{0, 0, 0},
				500,
				2,
				{GetRandomValue(0, 255), GetRandomValue(0, 255), GetRandomValue(0, 255), 255}
			};
			m_Bodies.push_back(b);
		}
	}

	void UpdateCamera()
	{
		Vector2 mousePositionDelta = GetMouseDelta();

		double angleX = -mousePositionDelta.x * mouseSensitivity;
		double angleY = -mousePositionDelta.y * mouseSensitivity;

		// Rotation axis
		Vector3 up = m_Camera.up;

		// View vector
		Vector3 targetPosition = Vector3Subtract(m_Camera.target, m_Camera.position);

		// Rotate view vector around up axis
		targetPosition = Vector3RotateByAxisAngle(targetPosition, up, angleX);

		// Move position relative to target
		if (CAMERA_ORBIT_MODE)
		{
			// Move position relative to target
			m_Camera.position = Vector3Subtract(m_Camera.target, targetPosition);
		}
		else // rotate around camera.position
		{
			// Move target relative to position
			m_Camera.target = Vector3Add(m_Camera.position, targetPosition);
		}

		// Rotation axis
		Vector3 right = Vector3CrossProduct(targetPosition, up);

		// Clamp view up
		float maxAngleUp = Vector3Angle(up, targetPosition);
		maxAngleUp -= 0.001f; // avoid numerical errors
		if (angleY > maxAngleUp) angleY = maxAngleUp;

		// Clamp view down
		float maxAngleDown = Vector3Angle(Vector3Negate(up), targetPosition);
		maxAngleDown *= -1.0f; // downwards angle is negative
		maxAngleDown += 0.001f; // avoid numerical errors
		if (angleY < maxAngleDown) angleY = maxAngleDown;
		
		// Rotate view vector around right axis
		targetPosition = Vector3RotateByAxisAngle(targetPosition, right, angleY);

		// Move position relative to target
		if (CAMERA_ORBIT_MODE)
		{
			// Move position relative to target
			m_Camera.position = Vector3Subtract(m_Camera.target, targetPosition);
		}
		else // rotate around camera.position
		{
			// Move target relative to position
			m_Camera.target = Vector3Add(m_Camera.position, targetPosition);
		}
	}

	void ProcessInputs() override
	{
		if (IsKeyPressed(KEY_SPACE))
		{
			m_isPaused = !m_isPaused;
			if (m_isPaused)
			{
				EnableCursor();
			}
			else
			{
				DisableCursor();
			}
		}

		if (IsKeyPressed(KEY_P))
		{
			m_isDebug = !m_isDebug;
		}

		UpdateCamera();
	}

	void FixedUpdate(double dt) override
	{
		// calculate accelerations and resolve collisions
		for (Body& b : m_Bodies)
		{
			b.acc = { 0, 0 };
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

		// update positions and velocities
		for (Body& b : m_Bodies)
		{
			b.vel = Vector3Add(b.vel, Vector3Scale(b.acc, dt));
			b.pos = Vector3Add(b.pos, Vector3Scale(b.vel, dt));

			if (Vector3Distance(b.pos, { 0, 0, 0 }) > 1000)
			{
				b.deleted = true;
			}
		}

		// Resolve collisions separately
		const int COLLISION_ITERATIONS = 2;  // Multiple iterations for stability
		for (int iter = 0; iter < COLLISION_ITERATIONS; iter++) {
			for (size_t i = 0; i < m_Bodies.size(); i++) {
				for (size_t j = i + 1; j < m_Bodies.size(); j++) {
					resolveCollision(m_Bodies[i], m_Bodies[j]);
				}
			}
		}
	}

	void Update(double dt) override
	{
		// remove bodies pending deletion
		std::erase_if(m_Bodies, [](const Body& body) { return body.deleted; });
	}

	void Draw() const override
	{
		BeginDrawing();
			ClearBackground(BLACK);

			BeginMode3D(m_Camera);
				SetCustomProjectionMatrix(m_Camera, 10000.0f); // Set far clipping plane to 10000.0f

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

		EndDrawing();
	}

private:
	std::vector<Body> m_Bodies;
	Camera3D m_Camera;
};

void main() {
	Version2 driver;
	driver.Run();
}
