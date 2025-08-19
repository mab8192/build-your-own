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

    // top
    scene.addObject(RigidBody2D()
        .setKinematic(false)
        .setShape(Shape::SQUARE)
        .setPosition(Vec2(0, 3))
        .setScale(Vec2(10, 0.2))
        .setMass(1.0)
        .setColor(GRAY));

    // bottom
    scene.addObject(RigidBody2D()
        .setKinematic(false)
        .setShape(Shape::SQUARE)
        .setPosition(Vec2(0, -3))
        .setScale(Vec2(10, 0.2))
        .setMass(1.0)
        .setColor(GRAY));

    // left
    scene.addObject(RigidBody2D()
        .setKinematic(false)
        .setShape(Shape::SQUARE)
        .setPosition(Vec2(-5, 0))
        .setScale(Vec2(0.2, 6))
        .setMass(1.0)
        .setColor(GRAY));

    // right
    scene.addObject(RigidBody2D()
        .setKinematic(false)
        .setShape(Shape::SQUARE)
        .setPosition(Vec2(5, 0))
        .setScale(Vec2(0.2, 6))
        .setMass(1.0)
        .setColor(GRAY));

    // Squares
    scene.addObject(RigidBody2D()
        .setShape(Shape::SQUARE)
        .setMass(5.0)
        .setPosition(Vec2(-3, 0))
        .setVel(Vec2(1, 0))
        .setColor(BLUE));

    scene.addObject(RigidBody2D()
        .setShape(Shape::SQUARE)
        .setMass(5.0)
        .setPosition(Vec2(-1, 0))
        .setVel(Vec2(-1, 0))
        .setColor(RED));

    while (!WindowShouldClose()) {
        // Update
        scene.update(GetFrameTime());

        // Draw
        BeginDrawing();
        ClearBackground(BLACK);

        for (auto& e : scene.bodies) {
            // Since +y in the world is up, need to flip the order of vertices so the triangles are rendered clockwise
            std::vector<Vector2> vertices;
            for (int i = e.vertices.size() - 1; i >= 0; i--) {
                const Vec2& v = e.transform.apply(e.vertices[i]);
                // Shift origin to center of screen and scale to pixel space
                Vec2 screenPos = Vec2(origin.x + v.x * resolution, origin.y - v.y * resolution);
                vertices.push_back(screenPos.toRaylib());
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
