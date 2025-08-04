#pragma once
#include "World.h"
#include "FreeCamera.h"

class Renderer {
public:
    Renderer();
    ~Renderer();
    void renderWorld(const FreeCamera& camera, const World& world);
};
