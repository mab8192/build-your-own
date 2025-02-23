#pragma once

#include "raylib.h"

void SetMouseSensitivity(float sens);
void SetCameraOrbit(bool orbit);
void SetCustomProjectionMatrix(Camera camera, float far);
void OrbitCamera(Camera& camera);
void FPCamera(Camera& camera);
void ControlCamera(Camera& camera);
