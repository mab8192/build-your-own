#pragma once

#include "World.h"
#include "Renderer.h"
#include "FreeCamera.h"
#include "Input.h"

struct Config
{
    int screenWidth = 1600;
    int screenHeight = 900;

    int renderDistance = 16; // How far the player can see in chunks
};

class Engine
{
public:
    Engine(const Config& config = Config());
    ~Engine();

    bool initialize();
    void run();
    void shutdown();

private:
    void update(float dt);
    void render();

private:
    Config m_Config;

    World m_World;
    FreeCamera m_Camera;
    Renderer m_Renderer;

    Input m_Input;

    bool m_Initialized;
    bool m_Running;
};
