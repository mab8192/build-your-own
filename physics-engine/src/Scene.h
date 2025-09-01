#pragma once
#include <vector>
#include <unordered_map>
#include <utility>
#include "RigidBody.h"
#include <iostream>


namespace Physics {

// Simple spatial hash grid for broad-phase collision
class Scene {
public:
    std::vector<RigidBody> bodies;
    double cellSize;

    Scene(double cellSize = 1.0);
    void addObject(const RigidBody& e);
    void update(double dt);
    void handleCollisions();
};

} // namespace Physics
