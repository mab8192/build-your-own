#include <vector>

#include "raylib.h"
#include "raymath.h"

#include "RigidBody.h"
#include "Scene.h"

using namespace Physics;

int main() {
    const int screenWidth = 1600;
    const int screenHeight = 900;
    const float resolution = 120; // Pixels per unit

    Vec2 origin = Vec2(screenWidth / 2.0f, screenHeight / 2.0f);

    InitWindow(screenWidth, screenHeight, "raylib + CMake + VS Code");
    SetTargetFPS(60);

    Scene scene(1.0);

    // Box around the whole scene
    scene.addObject(RigidBody()
        .setShape(Shape::SQUARE)
        .setPosition(Vec2(0, -4))
        .setScale(Vec2(20, 1))
        .setKinematic(false));

    scene.addObject(RigidBody()
        .setShape(Shape::SQUARE)
        .setPosition(Vec2(6, 0))
        .setScale(Vec2(1, 20))
        .setKinematic(false));

    scene.addObject(RigidBody()
        .setShape(Shape::SQUARE)
        .setPosition(Vec2(-6, 0))
        .setScale(Vec2(1, 20))
        .setKinematic(false));

    scene.addObject(RigidBody()
        .setShape(Shape::SQUARE)
        .setMass(1.0)
        // .setRotation(-0.2)
        .setVel(Vec2(0, -1))
        .setColor(BLUE));

    scene.addObject(RigidBody()
        .setShape(Shape::SQUARE)
        .setPosition(Vec2(0.4, -2))
        .setScale(Vec2(3, 0.1))
        .setKinematic(true));

    while (!WindowShouldClose()) {
        // Update
        scene.update(GetFrameTime());

        // Draw
        BeginDrawing();
        ClearBackground(BLACK);

        for (auto& e : scene.bodies) {
            if (e.isCircle) {
                Vec2 screenPos = Vec2(origin.x + e.transform.position.x * resolution, origin.y - e.transform.position.y * resolution);
                DrawEllipse(
                    static_cast<int>(screenPos.x),
                    static_cast<int>(screenPos.y),
                    static_cast<int>(0.5 * e.transform.scale.x * resolution),
                    static_cast<int>(0.5 * e.transform.scale.y * resolution),
                    e.material.color
                );
                continue;
            }

            // Since +y in the world is up, need to flip the order of vertices so the triangles are rendered clockwise
            std::vector<Vector2> vertices;
            for (int i = e.vertices.size() - 1; i >= 0; i--) {
                const Vec2& v = e.transform.apply(e.vertices[i]);
                // Shift origin to center of screen and scale to pixel space
                Vec2 screenPos = Vec2(origin.x + v.x * resolution, origin.y - v.y * resolution);
                vertices.push_back(screenPos.toRaylib());
            }

            Vec2 screenContactPoint = Vec2(origin.x + e.lastContactPoint.x * resolution, origin.y - e.lastContactPoint.y * resolution);
            Vec2 screenImpulsePoint = screenContactPoint + e.lastImpulse * 0.1 * resolution;
            DrawLineV(screenContactPoint.toRaylib(), screenImpulsePoint.toRaylib(), RED);

            DrawTriangleFan(
                vertices.data(),
                vertices.size(),
                e.material.color
            );
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
