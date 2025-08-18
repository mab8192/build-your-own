#pragma once

#include "Vec.h"

struct Transform2D
{
    Vec2 position;
    double rotation;
    Vec2 scale;

    Transform2D() : position(0, 0), rotation(0), scale(1, 1) {}
    Transform2D(const Vec2& pos, double rot, const Vec2& scl)
        : position(pos), rotation(rot), scale(scl) {}

    Transform2D& setPosition(const Vec2& pos) { position = pos; return *this; }
    Transform2D& setRotation(double rot) { rotation = rot; return *this; }
    Transform2D& setScale(const Vec2& scl) { scale = scl; return *this; }

    Vec2 apply(const Vec2& v) const {
        // Apply rotation and scale to the vector
        double cosTheta = std::cos(rotation);
        double sinTheta = std::sin(rotation);
        double x = v.x * cosTheta - v.y * sinTheta;
        double y = v.x * sinTheta + v.y * cosTheta;
        return Vec2(x * scale.x + position.x, y * scale.y + position.y);
    }
};
