#include "raylib.h"
#include "raymath.h"

#include "Engine.h"

Engine::Engine(const Config& config) 
    : m_Config(config), m_World(16, 16, 16), m_Camera(), m_Renderer(), m_Input(), m_Initialized(false), m_Running(false) 
{

}

Engine::~Engine()
{
    shutdown();
}

bool Engine::initialize()
{
    if (!m_Initialized) {
        InitWindow(m_Config.screenWidth, m_Config.screenHeight, "Voxel Engine");

        // allow resizing
        SetWindowState(FLAG_WINDOW_RESIZABLE);
        SetWindowMinSize(800, 600);

        // Capture mouse
        DisableCursor();

        SetTargetFPS(60);

        m_World = World(16, 16, 16); // Initialize world with default chunk size

        m_Camera = FreeCamera(); // Initialize camera with default values
        m_Camera.setPosition({0.0f, 10.0f, 10.0f}); // Set camera position above the ground
        m_Camera.setTarget({0.0f, 0.0f, 0.0f}); // Look at the origin
        m_Camera.setUp({0.0f, 1.0f, 0.0f}); // Set up vector
        m_Camera.setFov(60.0f); // Set field of view

        m_Renderer = Renderer(); // Initialize renderer

        m_Input = Input(); // Initialize input handler

        m_Initialized = true;
    }

    return true;
}

void Engine::run()
{
    if (!m_Initialized) {
        initialize();
    }

    m_Running = true;

    while (!WindowShouldClose() && m_Running) {
        update(GetFrameTime());
        render();
    }
}

void Engine::shutdown()
{
    if (m_Initialized) {
        m_Running = false;
    }
}

void Engine::update(float dt)
{
    if (m_Running) {
        m_Input.update(); // Update input states
        m_Camera.update(CAMERA_FREE); // Update camera based on input

        m_World.update(dt); // Update the world state
    }
}

void Engine::render()
{
    if (m_Running) {
        m_Renderer.renderWorld(m_Camera, m_World); // Render the world
    }
}
