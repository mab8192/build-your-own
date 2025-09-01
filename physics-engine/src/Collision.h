 #pragma once

#include "RigidBody.h"

namespace Physics {

void resolveCollision(RigidBody& a, RigidBody& b) {
    std::vector<Vec2> axes;
    // Calculate SAT axes to test (local edge, then rotate, then normalize)
    for (size_t i = 0; i < a.vertices.size(); ++i) {
        Vec2 v0 = a.vertices[i];
        Vec2 v1 = a.vertices[(i + 1) % a.vertices.size()];
        Vec2 edge = v1 - v0;
        Vec2 axis(-edge.y, edge.x); // Perpendicular to edge
        // Rotate axis by body's rotation
        double cosTheta = std::cos(a.transform.rotation);
        double sinTheta = std::sin(a.transform.rotation);
        Vec2 worldAxis(axis.x * cosTheta - axis.y * sinTheta, axis.x * sinTheta + axis.y * cosTheta);
        axes.push_back(worldAxis.normalized());
    }
    for (size_t i = 0; i < b.vertices.size(); ++i) {
        Vec2 v0 = b.vertices[i];
        Vec2 v1 = b.vertices[(i + 1) % b.vertices.size()];
        Vec2 edge = v1 - v0;
        Vec2 axis(-edge.y, edge.x);
        double cosTheta = std::cos(b.transform.rotation);
        double sinTheta = std::sin(b.transform.rotation);
        Vec2 worldAxis(axis.x * cosTheta - axis.y * sinTheta, axis.x * sinTheta + axis.y * cosTheta);
        axes.push_back(worldAxis.normalized());
    }

    // Track minimum overlap and axis
    double minOverlap = std::numeric_limits<double>::max();
    Vec2 n;
    double minAmin = 0, minAmax = 0, minBmin = 0, minBmax = 0;
    Vec2 minAxis;
    bool found = false;
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
        if (aMax < bMin || bMax < aMin) {
            return; // No collision
        }
        double overlap = std::min(aMax, bMax) - std::max(aMin, bMin);
        if (overlap < minOverlap) {
            minOverlap = overlap;
            n = axis;
            minAmin = aMin;
            minAmax = aMax;
            minBmin = bMin;
            minBmax = bMax;
            minAxis = axis;
            found = true;
        }
    }
    if (!found) return;

    // Find the deepest point of penetration for contact (improves torque/rotation response)
    double maxPenetration = -std::numeric_limits<double>::max();
    Vec2 contact(0, 0);
    for (const Vec2& v : a.vertices) {
        Vec2 worldV = a.transform.apply(v);
        double proj = worldV.dot(minAxis);
        double penetration = std::min(minAmax, minBmax) - proj;
        if (proj >= minBmin - 1e-6 && proj <= minBmax + 1e-6 && penetration > maxPenetration) {
            maxPenetration = penetration;
            contact = worldV;
        }
    }
    for (const Vec2& v : b.vertices) {
        Vec2 worldV = b.transform.apply(v);
        double proj = worldV.dot(minAxis);
        double penetration = std::min(minAmax, minBmax) - proj;
        if (proj >= minAmin - 1e-6 && proj <= minAmax + 1e-6 && penetration > maxPenetration) {
            maxPenetration = penetration;
            contact = worldV;
        }
    }
    Vec2 p = contact;
    if (maxPenetration == -std::numeric_limits<double>::max()) {
        // Fallback: use center between shapes if no points found
        p = (a.transform.position + b.transform.position) * 0.5;
    }

    // Ensure the normal points from a to b at the contact point
    Vec2 ab = b.transform.position - a.transform.position;
    if (ab.dot(n) < 0) n = n * -1.0;

    // Resolve collision by moving only movable (kinematic) bodies
    double totalInvMass = (a.kinematic ? a.invMass : 0.0) + (b.kinematic ? b.invMass : 0.0);
    if (totalInvMass > 0) {
        double aMove = (a.kinematic ? a.invMass / totalInvMass : 0.0) * minOverlap;
        double bMove = (b.kinematic ? b.invMass / totalInvMass : 0.0) * minOverlap;
        if (a.kinematic) a.transform.position -= n * aMove;
        if (b.kinematic) b.transform.position += n * bMove;
    }
    
    // Apply impulse
    Vec2 ra = p - a.transform.position;
    Vec2 rb = p - b.transform.position;
    // Relative velocity at contact
    Vec2 va = a.vel + ra.perpendicular() * a.angularVelocity;
    Vec2 vb = b.vel + rb.perpendicular() * b.angularVelocity;
    Vec2 rv = vb - va;
    double velAlongNormal = rv.dot(n);

    if (velAlongNormal > 0) return; // Bodies are separating

    double e = std::min(a.material.restitution, b.material.restitution);

    double raCrossN = ra.cross(n);
    double rbCrossN = rb.cross(n);

    double invMassSum = 0.0;
    if (a.kinematic) invMassSum += a.invMass + (raCrossN * raCrossN) * a.invInertia;
    if (b.kinematic) invMassSum += b.invMass + (rbCrossN * rbCrossN) * b.invInertia;
    if (invMassSum == 0) return;

    double j = -(1 + e) * velAlongNormal / invMassSum;
    Vec2 impulse = n * j;

    // Apply impulse only to kinematic bodies, with correct sign
    if (a.kinematic) a.impulse(-impulse, p);
    if (b.kinematic) b.impulse(impulse, p);
}

} // namespace Physics
