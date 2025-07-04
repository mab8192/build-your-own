#pragma once

#include "raylib.h"

namespace ControlSettings
{
	static float mouseSensitivity = 0.003f;
	static bool cameraOrbitMode = true;
}

void SetMouseSensitivity(float sens);
void SetCameraOrbit(bool orbit);
void SetCustomProjectionMatrix(Camera camera, float far);
void OrbitCamera(Camera& camera);
void FPCamera(Camera& camera);
