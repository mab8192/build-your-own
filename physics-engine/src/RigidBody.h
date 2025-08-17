#pragma once

#include "Vec.h"

struct RigidBody
{
    double mass;
    Vec2 pos, vel, acc;

    RigidBody(double mass = 1.0, Vec2 pos = Vec2(), Vec2 vel = Vec2(), Vec2 acc = Vec2())
        : mass(mass), pos(pos), vel(vel), acc(acc) 
    {
        
    }

    void applyForce(const Vec2& force) { acc += force / mass; }

    void update(double dt)
    {
        vel += acc * dt;
        pos += vel * dt;
        acc.x = 0;
        acc.y = 0;
    }
};
