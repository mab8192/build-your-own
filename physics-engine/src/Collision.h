#pragma once

#include "RigidBody.h"

void resolveCollision(RigidBody2D& a, RigidBody2D& b) {
    std::vector<Vec2> axes;

    // Calculate SAT axes to test
    for (size_t i = 0; i < a.vertices.size(); ++i) {
        Vec2 edge = a.transform.apply(a.vertices[(i + 1) % a.vertices.size()]) - a.transform.apply(a.vertices[i]);
        Vec2 axis(-edge.y, edge.x); // Perpendicular to edge
        axes.push_back(axis.normalized());
    }

    for (size_t i = 0; i < b.vertices.size(); ++i) {
        Vec2 edge = b.transform.apply(b.vertices[(i + 1) % b.vertices.size()]) - b.transform.apply(b.vertices[i]);
        Vec2 axis(-edge.y, edge.x); // Perpendicular to edge
        axes.push_back(axis.normalized());
    }

    // Separating axis theorem and impulse response
    // Track minimum overlap and axis
    double minOverlap = std::numeric_limits<double>::max();
    Vec2 minAxis;
    for (const Vec2& axis : axes) {
        double aMin = std::numeric_limits<double>::max();
        double aMax = std::numeric_limits<double>::lowest();
        for (const Vec2& v : a.vertices) {
            double proj = a.transform.apply(v).dot(axis);
            aMin = std::min(aMin, proj);
            aMax = std::max(aMax, proj);
        }
        double bMin = std::numeric_limits<double>::max();
        double bMax = std::numeric_limits<double>::lowest();
        for (const Vec2& v : b.vertices) {
            double proj = b.transform.apply(v).dot(axis);
            bMin = std::min(bMin, proj);
            bMax = std::max(bMax, proj);
        }
        // SAT: If there is a gap, no collision
        if (aMax < bMin || bMax < aMin) {
            return; // No collision
        }
        // Find overlap on this axis
        double overlap = std::min(aMax, bMax) - std::max(aMin, bMin);
        if (overlap < minOverlap) {
            minOverlap = overlap;
            minAxis = axis;
        }
    }
    // If we get here, collision detected. Use minAxis and minOverlap for resolution

    // Ensure the axis points from a to b
    Vec2 d = b.transform.position - a.transform.position;
    if (d.dot(minAxis) < 0) minAxis = minAxis * -1.0;

    // Resolve collision by moving the bodies apart proportionally to their mass
    if (a.kinematic && !b.kinematic) {
        a.transform.position -= minAxis * minOverlap;
    } else if (!a.kinematic && b.kinematic) {
        b.transform.position += minAxis * minOverlap;
    } else {
        double totalMass = a.mass + b.mass;
        double aMove = (b.mass / totalMass) * minOverlap;
        double bMove = (a.mass / totalMass) * minOverlap;
        a.transform.position -= minAxis * aMove;
        b.transform.position += minAxis * bMove;
    }

    // Apply impulse response (elastic collision, restitution = 1)
    Vec2 relativeVelocity = b.vel - a.vel;
    double velAlongNormal = relativeVelocity.dot(minAxis);
    if (velAlongNormal > 0) return; // They are moving apart

    double restitution = 1.0; // perfectly elastic
    double impulseMag = -(1.0 + restitution) * velAlongNormal / (1.0 / a.mass + 1.0 / b.mass);
    Vec2 impulse = minAxis * impulseMag;
    if (a.kinematic) a.impulse(impulse * -1);
    if (b.kinematic) b.impulse(impulse);
}
