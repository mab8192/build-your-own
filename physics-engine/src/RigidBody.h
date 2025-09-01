#pragma once

#include <vector>
#include "Vec.h"
#include "Transform.h"


namespace Physics {

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

struct Mat {
    double staticFriction = 0.2;
    double dynamicFriction = 0.1;
    double restitution = 0.9; // bounciness
    Color color = WHITE;
};

struct RigidBody {
    double mass = 1.0;
    double invMass = 1.0;
    double inertia = 1.0; // moment of inertia
    double invInertia = 1.0;
    Physics::Transform transform;
    Physics::Vec2 vel, acc;
    Physics::Vec2 centerOfMass = Physics::Vec2(0, 0);
    double angularVelocity = 0.0;
    double angularAcc = 0.0;
    double torque = 0.0;
    bool kinematic = true;
    bool isCircle = false;
    Mat material;
    std::vector<Physics::Vec2> vertices;

    Vec2 lastImpulse, lastContactPoint;

    bool resolved = false; // Whether the shape has been resolved (inertia, center of mass)

    RigidBody();

    RigidBody& setMass(double m);
    RigidBody& setTransform(const Physics::Transform& t) { transform = t; return *this; }
    RigidBody& setPosition(const Physics::Vec2& p) { transform.position = p; return *this; }
    RigidBody& setRotation(double r) { transform.rotation = r; return *this; }
    RigidBody& setScale(const Physics::Vec2& s) { transform.scale = s; return *this; }
    RigidBody& setKinematic(bool k) { kinematic = k; return *this; }
    RigidBody& setVel(const Physics::Vec2& v) { vel = v; return *this; }
    RigidBody& setMaterial(const Mat& m) { material = m; return *this; }
    RigidBody& setColor(const Color& c) { material.color = c; return *this; }
    RigidBody& addVertex(const Physics::Vec2& v) { vertices.push_back(v); return *this; }

    RigidBody& addVertices(const std::vector<Physics::Vec2>& vs);
    RigidBody& setVertices(const std::vector<Physics::Vec2>& vs);
    RigidBody& setVertices(const double* vs, size_t count);
    RigidBody& setShape(Shape shape);

    // Force interactions
    void applyForce(const Physics::Vec2& force);
    void applyForce(const Physics::Vec2& force, const Physics::Vec2& point);
    void applyTorque(double t);
    void impulse(const Physics::Vec2& impulse, const Physics::Vec2& contactVector = Physics::Vec2(0,0));

    // Physics Step
    void update(double dt);

    // Object resolution
    void resolve();
    void computeCenterOfMass();
    void computeInertia();
};

} // namespace Physics
