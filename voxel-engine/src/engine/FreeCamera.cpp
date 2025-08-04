#include "FreeCamera.h"

FreeCamera::FreeCamera() {
    // Initialize camera with default values
    setPosition({0.0f, 0.0f, 0.0f});
    setTarget({0.0f, 0.0f, -1.0f});
    setUp({0.0f, 1.0f, 0.0f});
    setFov(45.0f);
    setProjection(CAMERA_PERSPECTIVE);
}

FreeCamera::~FreeCamera() {}

void FreeCamera::setPosition(const Vector3& position) {
    m_Camera.position = position;
}

void FreeCamera::setTarget(const Vector3& target) {
    m_Camera.target = target;
}

void FreeCamera::setUp(const Vector3& up) {
    m_Camera.up = up;
}

void FreeCamera::setFov(float fov) {
    m_Camera.fovy = fov;
}

void FreeCamera::setProjection(CameraProjection projection) {
    m_Camera.projection = projection;
}

Camera3D FreeCamera::getCamera() const {
    return m_Camera;
}

void FreeCamera::update(int mode) {
    UpdateCamera(&m_Camera, mode);
}
