#include <vector>

#include "raylib.h"
#include "raymath.h"

#include "RigidBody.h"
#include "Scene.h"


int main() {
    const int screenWidth = 1600;
    const int screenHeight = 900;
    const float resolution = 120; // Pixels per unit

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

    // Ramp
    scene.addObject(RigidBody2D()
        .setKinematic(false)
        .addVertex(Vec2(-2, -0.1))
        .addVertex(Vec2(1, 1))
        .addVertex(Vec2(1, -0.1))
        .setPosition(Vec2(2, -1))
        .setMass(1.0)
        .setColor(GRAY));

    // Squares
    scene.addObject(RigidBody2D()
        .setShape(Shape::SQUARE)
        .setMass(5.0)
        .setPosition(Vec2(-3, 0))
        .setVel(Vec2(3, 1))
        .setColor(BLUE));

    scene.addObject(RigidBody2D()
        .setShape(Shape::SQUARE)
        .setMass(2.0)
        .setPosition(Vec2(-1, 0))
        .setVel(Vec2(1, -2))
        .setColor(RED));

    scene.addObject(RigidBody2D()
        .setShape(Shape::SQUARE)
        .setMass(3.0)
        .setPosition(Vec2(3, -1))
        .setVel(Vec2(-1, -2))
        .setColor(GREEN));

    scene.addObject(RigidBody2D()
        .setShape(Shape::SQUARE)
        .setMass(20.0)
        .setPosition(Vec2(0, 1))
        .setVel(Vec2(0, -2))
        .setColor(YELLOW));

    scene.addObject(RigidBody2D()
        .setShape(Shape::CIRCLE)
        .setScale(Vec2(2, 1))
        .setColor(WHITE));


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
                    e.color
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
