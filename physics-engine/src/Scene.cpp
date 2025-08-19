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
	for (size_t i = 0; i < bodies.size() - 1; ++i) {
		for (size_t j = i + 1; j < bodies.size(); ++j) {
			if (!bodies[i].kinematic && !bodies[j].kinematic) continue;
			resolveCollision(bodies[i], bodies[j]);
		}
	}
}
