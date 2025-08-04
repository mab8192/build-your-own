#pragma once

#include "raylib.h"

// First person Wrapper for raylib camera functionality
class FreeCamera
{
public:
    FreeCamera();
    ~FreeCamera();

    void setPosition(const Vector3& position);
    void setTarget(const Vector3& target);
    void setUp(const Vector3& up);
    void setFov(float fov);
    void setProjection(CameraProjection projection);

    Camera3D getCamera() const;

    void update(int mode);

private:
    Camera3D m_Camera; // Using raylib's Camera3D type
};
