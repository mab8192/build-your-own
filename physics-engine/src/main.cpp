#include <vector>

#include "raylib.h"
#include "raymath.h"

#include "RigidBody.h"
#include "Scene.h"


int main() {
    const int screenWidth = 1600;
    const int screenHeight = 900;
    const float resolution = 100; // Pixels per unit

    Vec2 origin = Vec2(screenWidth / 2.0f, screenHeight / 2.0f);

    InitWindow(screenWidth, screenHeight, "raylib + CMake + VS Code");
    SetTargetFPS(60);

    Scene scene(1.0);

    scene.addObject(RigidBody2D()
        .setKinematic(false)
        .setShape(Shape::SQUARE)
        .setPosition(Vec2(0, 1))
        .setScale(Vec2(5, 1))
        .setMass(1.0)
        .setColor(GRAY));

    scene.addObject(RigidBody2D()
        .setShape(Shape::SQUARE)
        .setMass(5.0)
        .setPosition(Vec2(-1, 0))
        .setVel(Vec2(0.5, 0.5))
        .setColor(WHITE));

    scene.addObject(RigidBody2D()
        .setShape(Shape::TRIANGLE)
        .setPosition(Vec2(1, 0))
        .setVel(Vec2(-0.5, 0.5))
        .setColor(BLUE));

    while (!WindowShouldClose()) {
        // Update
        scene.update(GetFrameTime());

        // Draw
        BeginDrawing();
        ClearBackground(BLACK);

        for (auto& e : scene.bodies) {
            Vec2 center = Vec2(origin.x + e.transform.position.x * resolution, origin.y - e.transform.position.y * resolution);
            
            // Since +y in the world is up, need to flip the order of vertices so the triangles are rendered clockwise
            std::vector<Vector2> vertices;
            for (int i = e.vertices.size() - 1; i >= 0; i--) {
                const Vec2& v = e.transform.apply(e.vertices[i]);
                Vec2 worldPos = Vec2(center.x + v.x * resolution, center.y - v.y * resolution);
                vertices.push_back(worldPos.toRaylib());
            }

            DrawTriangleFan(
                vertices.data(),
                vertices.size(),
                e.color
            );
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
