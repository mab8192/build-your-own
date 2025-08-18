#pragma once
#include <vector>
#include <unordered_map>
#include <utility>
#include "RigidBody.h"
#include <iostream>

// Simple spatial hash grid for broad-phase collision
class Scene {
public:
    std::vector<RigidBody2D> bodies;
    double cellSize;

    Scene(double cellSize = 1.0);
    void addObject(const RigidBody2D& e);
    void update(double dt);
    void handleCollisions();

private:
    struct pair_hash {
        std::size_t operator()(const std::pair<int, int>& p) const {
            return std::hash<int>()(p.first) ^ (std::hash<int>()(p.second) << 1);
        }
    };

    std::pair<int, int> getCell(const Vec2& pos) const {
        return { static_cast<int>(pos.x / cellSize), static_cast<int>(pos.y / cellSize) };
    }
};
