#pragma once

#include "Vec.h"
#include "RigidBody.h"

struct Entity
{
    RigidBody body;
    double scale = 1.0;
    Color color = RED;

    Entity(double mass = 1.0, Vec2 pos = Vec2(), Vec2 vel = Vec2(), Vec2 acc = Vec2())
        : body(mass, pos, vel, acc) {}

    Entity(double mass, Vec2 pos, Vec2 vel, Vec2 acc, double scale, Color color)
        : body(mass, pos, vel, acc), scale(scale), color(color) {}

    Vec2 getPosition() const {
        return body.pos;
    }

    void applyForce(const Vec2& force) {
        body.applyForce(force);
    }

    void update(double dt) {
        body.update(dt);
    }
};
