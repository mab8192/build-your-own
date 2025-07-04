#include "Camera.h"

#include "rlgl.h";
#include "raymath.h"

void SetMouseSensitivity(float sens)
{
	ControlSettings::mouseSensitivity = sens;
}

void SetCameraOrbit(bool orbit)
{
	ControlSettings::cameraOrbitMode = orbit;
}

void SetCustomProjectionMatrix(Camera camera, float far) 
{
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

void OrbitCamera(Camera& camera)
{
	Vector2 mousePositionDelta = GetMouseDelta();

	double angleX = -mousePositionDelta.x * ControlSettings::mouseSensitivity;
	double angleY = -mousePositionDelta.y * ControlSettings::mouseSensitivity;

	Vector3 up = camera.up;
	Vector3 targetPosition = Vector3Subtract(camera.target, camera.position);

	targetPosition = Vector3RotateByAxisAngle(targetPosition, up, angleX);
	camera.position = Vector3Subtract(camera.target, targetPosition);

	Vector3 right = Vector3CrossProduct(targetPosition, up);

	float maxAngleUp = Vector3Angle(up, targetPosition);
	maxAngleUp -= 0.001f; // avoid numerical errors
	if (angleY > maxAngleUp) angleY = maxAngleUp;

	float maxAngleDown = Vector3Angle(Vector3Negate(up), targetPosition);
	maxAngleDown *= -1.0f; // downwards angle is negative
	maxAngleDown += 0.001f; // avoid numerical errors
	if (angleY < maxAngleDown) angleY = maxAngleDown;

	targetPosition = Vector3RotateByAxisAngle(targetPosition, right, angleY);
	camera.position = Vector3Subtract(camera.target, targetPosition);
}

void FPCamera(Camera& camera)
{
	Vector2 mousePositionDelta = GetMouseDelta();

	double angleX = -mousePositionDelta.x * ControlSettings::mouseSensitivity;
	double angleY = -mousePositionDelta.y * ControlSettings::mouseSensitivity;

	Vector3 up = camera.up;
	Vector3 targetPosition = Vector3Subtract(camera.target, camera.position);

	targetPosition = Vector3RotateByAxisAngle(targetPosition, up, angleX);
	camera.target = Vector3Add(camera.position, targetPosition);

	Vector3 right = Vector3CrossProduct(targetPosition, up);

	float maxAngleUp = Vector3Angle(up, targetPosition);
	maxAngleUp -= 0.001f; // avoid numerical errors
	if (angleY > maxAngleUp) angleY = maxAngleUp;

	float maxAngleDown = Vector3Angle(Vector3Negate(up), targetPosition);
	maxAngleDown *= -1.0f; // downwards angle is negative
	maxAngleDown += 0.001f; // avoid numerical errors
	if (angleY < maxAngleDown) angleY = maxAngleDown;

	targetPosition = Vector3RotateByAxisAngle(targetPosition, right, angleY);
	camera.target = Vector3Add(camera.position, targetPosition);
}