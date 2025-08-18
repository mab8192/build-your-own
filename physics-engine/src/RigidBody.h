#pragma once

#include <vector>
#include "Vec.h"
#include "Transform.h"

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
    TRIANGLE
};

struct RigidBody2D
{
    double mass = 1.0;
    Transform2D transform;
    Vec2 vel, acc;
    bool kinematic = true; // If true, the body is kinematic and will not be affected by forces

    Color color = WHITE;
    
    std::vector<Vec2> vertices; // Polygon vertices in local space

    RigidBody2D();

    RigidBody2D& setMass(double m);
    RigidBody2D& setTransform(const Transform2D& t);
    RigidBody2D& setPosition(const Vec2& p);
    RigidBody2D& setRotation(double r);
    RigidBody2D& setScale(const Vec2& s);
    RigidBody2D& setKinematic(bool k);
    RigidBody2D& setVel(const Vec2& v);
    RigidBody2D& acceleration(const Vec2& a);
    RigidBody2D& setColor(const Color& c);
    RigidBody2D& addVertex(const Vec2& v);
    RigidBody2D& addVertices(const std::vector<Vec2>& vs);
    RigidBody2D& setVertices(const std::vector<Vec2>& vs);
    RigidBody2D& setVertices(const double* vs, size_t count);
    RigidBody2D& setShape(Shape shape);

    void applyForce(const Vec2& force);
    void impulse(const Vec2& impulse);
    void update(double dt);
};
