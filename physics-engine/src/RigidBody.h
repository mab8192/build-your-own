#pragma once

#include <vector>
#include "Vec.h"
#include "Transform.h"

// Default Shapes
const double SQUARE_VERTICES[] = {
    -0.5, -0.5,
    -0.5,  0.5,
     0.5,  0.5,
     0.5, -0.5
};

const double TRIANGLE_VERTICES[] = {
     0.0,  0.57735026919,  // Top vertex (height = sqrt(3)/2 * side, side=1)
     0.5, -0.28867513459,  // Bottom right
    -0.5, -0.28867513459   // Bottom left
};

enum Shape {
    SQUARE,
    TRIANGLE,
    CIRCLE,
};

struct RigidBody2D
{
    double mass = 1.0;
    Transform2D transform;
    Vec2 vel, acc;
    bool kinematic = true; // If true, the body is kinematic and will experience force, update position/velocity, etc.

    double dynamicFriction = 0.03;
    double staticFriction = 0.05;

    bool isCircle = false;
    Color color = WHITE;
    
    std::vector<Vec2> vertices; // Polygon vertices in local space

    RigidBody2D();

    RigidBody2D& setMass(double m) { mass = m; return *this; }
    RigidBody2D& setTransform(const Transform2D& t) { transform = t; return *this; }
    RigidBody2D& setPosition(const Vec2& p) { transform.position = p; return *this; }
    RigidBody2D& setRotation(double r) { transform.rotation = r; return *this; }
    RigidBody2D& setScale(const Vec2& s) { transform.scale = s; return *this; }
    RigidBody2D& setKinematic(bool k) { kinematic = k; return *this; }
    RigidBody2D& setVel(const Vec2& v) { vel = v; return *this; }
    RigidBody2D& setDynamicFriction(double f) { dynamicFriction = f; return *this; }
    RigidBody2D& setStaticFriction(double f) { staticFriction = f; return *this; }
    RigidBody2D& setColor(const Color& c) { color = c; return *this; }
    RigidBody2D& addVertex(const Vec2& v) { vertices.push_back(v); return *this; }

    RigidBody2D& addVertices(const std::vector<Vec2>& vs);
    RigidBody2D& setVertices(const std::vector<Vec2>& vs);
    RigidBody2D& setVertices(const double* vs, size_t count);
    RigidBody2D& setShape(Shape shape);

    void applyForce(const Vec2& force);
    void impulse(const Vec2& impulse);
    void update(double dt);
};
