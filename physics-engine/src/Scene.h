#pragma once
#include <vector>
#include <unordered_map>
#include <utility>
#include "Entity.h"

// Simple spatial hash grid for broad-phase collision
class Scene {
public:
    std::vector<Entity> entities;
    double cellSize;

    Scene(double cellSize = 1.0) : cellSize(cellSize) {}

    void addObject(const Entity& e) {
        entities.push_back(e);
    }

    void update(double dt) {
        for (auto& e : entities) {
            e.update(dt);
        }
        handleCollisions();
    }

    void handleCollisions() {
        // Spatial hash: map from cell coordinate to indices of entities in that cell
        std::unordered_map<std::pair<int, int>, std::vector<int>, pair_hash> grid;
        for (int i = 0; i < entities.size(); ++i) {
            auto cell = getCell(entities[i].getPosition());
            grid[cell].push_back(i);
        }

        // Check collisions within each cell
        for (const auto& entry : grid) {
            const auto& indices = entry.second;
            for (size_t i = 0; i < indices.size(); ++i) {
                for (size_t j = i + 1; j < indices.size(); ++j) {
                    resolveCollision(entities[indices[i]], entities[indices[j]]);
                }
            }
        }
    }

private:
    struct pair_hash {
        std::size_t operator()(const std::pair<int, int>& p) const {
            return std::hash<int>()(p.first) ^ (std::hash<int>()(p.second) << 1);
        }
    };

    std::pair<int, int> getCell(const Vec2& pos) const {
        return { static_cast<int>(pos.x / cellSize), static_cast<int>(pos.y / cellSize) };
    }

    void resolveCollision(Entity& a, Entity& b) {
        // Separating axis theorem and impule response

    }
};
