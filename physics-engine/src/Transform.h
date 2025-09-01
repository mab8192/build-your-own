#pragma once

#include "Vec.h"


namespace Physics {

struct Transform {
    Vec2 position;
    double rotation;
    Vec2 scale;

    Transform() : position(0, 0), rotation(0), scale(1, 1) {}
    Transform(const Vec2& pos, double rot, const Vec2& scl)
        : position(pos), rotation(rot), scale(scl) {}

    Transform& setPosition(const Vec2& pos) { position = pos; return *this; }
    Transform& setRotation(double rot) { rotation = rot; return *this; }
    Transform& setScale(const Vec2& scl) { scale = scl; return *this; }

    Vec2 apply(const Vec2& v) const {
        // Scale first
        double sx = v.x * scale.x;
        double sy = v.y * scale.y;
        // Then rotate
        double cosTheta = std::cos(rotation);
        double sinTheta = std::sin(rotation);
        double rx = sx * cosTheta - sy * sinTheta;
        double ry = sx * sinTheta + sy * cosTheta;
        // Then translate
        return Vec2(rx + position.x, ry + position.y);
    }
};

} // namespace Physics
