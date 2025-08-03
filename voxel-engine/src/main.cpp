#include "raylib.h"

int main()
{
    // Initialize window
    const int screenWidth = 800;
    const int screenHeight = 600;
    
    InitWindow(screenWidth, screenHeight, "Voxel Engine - Spinning Cube");
    SetTargetFPS(60);
    
    // Camera setup
    Camera3D camera = { 0 };
    camera.position = Vector3{ 0.0f, 2.0f, 4.0f };
    camera.target = Vector3{ 0.0f, 0.0f, 0.0f };
    camera.up = Vector3{ 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;
    
    // Cube rotation
    float rotation = 0.0f;
    
    // Main game loop
    while (!WindowShouldClose())
    {
        // Update
        rotation += 1.0f;
        
        // Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);
        
        BeginMode3D(camera);
        
        // Draw spinning cube
        DrawCube(Vector3{ 0.0f, 0.0f, 0.0f }, 1.0f, 1.0f, 1.0f, RED);
        DrawCubeWires(Vector3{ 0.0f, 0.0f, 0.0f }, 1.0f, 1.0f, 1.0f, BLACK);
        
        // Draw grid
        DrawGrid(10, 1.0f);
        
        EndMode3D();
        
        // Draw UI
        DrawText("Voxel Engine - Spinning Cube", 10, 10, 20, BLACK);
        DrawText("Press ESC to exit", 10, 30, 20, GRAY);
        DrawFPS(10, 50);
        
        EndDrawing();
    }
    
    // Cleanup
    CloseWindow();
    
    return 0;
} 