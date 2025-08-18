#pragma once

#include "RigidBody.h"

void resolveCollision(RigidBody2D& a, RigidBody2D& b) {
    // Separating axis theorem and impulse response
    Vec2 axis = b.transform.position - a.transform.position;
    Vec2 normal = axis.normalized();

    // Project vertices onto the normal to find the overlap
    double aMin = std::numeric_limits<double>::max();
    double aMax = std::numeric_limits<double>::lowest();
    for (const Vec2& v : a.vertices) {
        double proj = (a.transform.position + a.transform.apply(v)).dot(normal);
        aMin = std::min(aMin, proj);
        aMax = std::max(aMax, proj);
    }

    double bMin = std::numeric_limits<double>::max();
    double bMax = std::numeric_limits<double>::lowest();
    for (const Vec2& v : b.vertices) {
        double proj = (b.transform.position + b.transform.apply(v)).dot(normal);
        bMin = std::min(bMin, proj);
        bMax = std::max(bMax, proj);
    }

    // Check for overlap (if no overlap, return)
    if (aMax < bMin || bMax < aMin) {
        return;
    }

    // Calculate overlap (minimum translation distance)
    double overlap = std::min(aMax, bMax) - std::max(aMin, bMin);
    if (overlap <= 0) return;

    // Resolve collision by moving the bodies apart proportionally to their mass
    if (a.kinematic && !b.kinematic) {
        // If only a is kinematic
        a.transform.position -= normal * overlap;
    } else if (!a.kinematic && b.kinematic) {
        // If only b is kienmatic
        b.transform.position += normal * overlap;
    } else {
        // If both are kinematic, move them apart proportionally
        double totalMass = a.mass + b.mass;
        double aMove = (b.mass / totalMass) * overlap;
        double bMove = (a.mass / totalMass) * overlap;
        a.transform.position -= normal * aMove;
        b.transform.position += normal * bMove;
    }

    // Apply impulse response (elastic collision, restitution = 1)
    Vec2 relativeVelocity = b.vel - a.vel;
    double velAlongNormal = relativeVelocity.dot(normal);
    if (velAlongNormal > 0) return; // They are moving apart

    double restitution = 1.0; // perfectly elastic
    double impulseMag = -(1.0 + restitution) * velAlongNormal / (1.0 / a.mass + 1.0 / b.mass);
    Vec2 impulse = normal * impulseMag;
    
    if (a.kinematic) a.impulse(impulse * -1);
    if (b.kinematic) b.impulse(impulse);
}
