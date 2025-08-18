#include "Scene.h"
#include "Collision.h"

Scene::Scene(double cellSize) : cellSize(cellSize) {}

void Scene::addObject(const RigidBody2D& e) {
	bodies.push_back(e);
}

void Scene::update(double dt) {
	for (auto& e : bodies) {
		e.update(dt);
	}
	handleCollisions();
}

void Scene::handleCollisions() {
	// Spatial hash: map from cell coordinate to indices of bodies in that cell
	std::unordered_map<std::pair<int, int>, std::vector<int>, Scene::pair_hash> grid;
	for (int i = 0; i < bodies.size(); ++i) {
		auto cell = getCell(bodies[i].transform.position);
		grid[cell].push_back(i);
	}

	// Check collisions within each cell
	for (const auto& entry : grid) {
		const auto& indices = entry.second;
		for (size_t i = 0; i < indices.size(); ++i) {
			for (size_t j = i + 1; j < indices.size(); ++j) {
				resolveCollision(bodies[indices[i]], bodies[indices[j]]);
			}
		}
	}
}
