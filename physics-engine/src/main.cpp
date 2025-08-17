#include <vector>

#include "raylib.h"
#include "raymath.h"

#include "Entity.h"
#include "Scene.h"


int main() {
    const int screenWidth = 1600;
    const int screenHeight = 900;
    const float resolution = 100; // Pixels per unit

    Vec2 origin = Vec2(screenWidth / 2.0f, screenHeight / 2.0f);

    InitWindow(screenWidth, screenHeight, "raylib + CMake + VS Code");
    SetTargetFPS(60);

    Scene scene(1.0);

    scene.addObject(Entity(1.0, Vec2(-2, 0.5), Vec2(0.3, 0.2), Vec2(0, 0), 1, RED));
    scene.addObject(Entity(1.0, Vec2(1, 0.5), Vec2(-0.3, 0.2), Vec2(0, 0), 1, BLUE));

    while (!WindowShouldClose()) {
        // Update
        scene.update(GetFrameTime());

        // Draw
        BeginDrawing();
        ClearBackground(BLACK);

        for (auto& e : scene.entities) {
            Vec2 pos = Vec2(origin.x + e.getPosition().x * resolution, origin.y - e.getPosition().y * resolution);
            Vec2 scale = Vec2(e.scale, e.scale) * resolution;
            DrawRectangleV(pos.toRaylib(), scale.toRaylib(), e.color);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
