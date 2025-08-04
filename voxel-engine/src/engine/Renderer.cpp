#include "raylib.h"

#include "Renderer.h"


Renderer::Renderer() {}
Renderer::~Renderer() {}
void Renderer::renderWorld(const FreeCamera& camera, const World& world) 
{
    BeginDrawing();
    ClearBackground(BLACK);

    BeginMode3D(camera.getCamera());
    DrawGrid(100, 1.0f); // Draw a grid for reference
    
    EndMode3D();


    DrawFPS(10, 10);
    EndDrawing();
}
