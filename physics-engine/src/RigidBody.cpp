#include "RigidBody.h"

RigidBody2D::RigidBody2D() {}

RigidBody2D& RigidBody2D::addVertices(const std::vector<Vec2>& vs) {
	vertices.insert(vertices.end(), vs.begin(), vs.end());
	return *this;
}
RigidBody2D& RigidBody2D::setVertices(const std::vector<Vec2>& vs) {
	vertices = vs;
	return *this;
}
RigidBody2D& RigidBody2D::setVertices(const double* vs, size_t count) {
	vertices.clear();
	for (size_t i = 0; i < count; i += 2) {
		vertices.emplace_back(vs[i], vs[i + 1]);
	}
	return *this;
}
RigidBody2D& RigidBody2D::setShape(Shape shape) {
	vertices.clear();
	switch (shape) {
		case SQUARE:
			return setVertices(SQUARE_VERTICES, sizeof(SQUARE_VERTICES) / sizeof(SQUARE_VERTICES[0]));
		case TRIANGLE:
			return setVertices(TRIANGLE_VERTICES, sizeof(TRIANGLE_VERTICES) / sizeof(TRIANGLE_VERTICES[0]));
		case CIRCLE:
			isCircle = true;
			return *this;
	}
	return *this;
}
void RigidBody2D::applyForce(const Vec2& force) { acc += force / mass; }
void RigidBody2D::impulse(const Vec2& impulse) { vel += impulse / mass; }
void RigidBody2D::update(double dt) {
	if (!kinematic) return; // Non-kinematic bodies do not move automatically
	vel += acc * dt;
	transform.position += vel * dt;
	acc.x = 0;
	acc.y = 0;
}
