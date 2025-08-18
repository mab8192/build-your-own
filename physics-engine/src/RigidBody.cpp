#include "RigidBody.h"

RigidBody2D::RigidBody2D() {}

RigidBody2D& RigidBody2D::setMass(double m) { mass = m; return *this; }
RigidBody2D& RigidBody2D::setTransform(const Transform2D& t) { transform = t; return *this; }
RigidBody2D& RigidBody2D::setPosition(const Vec2& p) { transform.position = p; return *this; }
RigidBody2D& RigidBody2D::setRotation(double r) { transform.rotation = r; return *this; }
RigidBody2D& RigidBody2D::setScale(const Vec2& s) { transform.scale = s; return *this; }
RigidBody2D& RigidBody2D::setKinematic(bool k) { kinematic = k; return *this; }
RigidBody2D& RigidBody2D::setVel(const Vec2& v) { vel = v; return *this; }
RigidBody2D& RigidBody2D::acceleration(const Vec2& a) { acc = a; return *this; }
RigidBody2D& RigidBody2D::setColor(const Color& c) { color = c; return *this; }
RigidBody2D& RigidBody2D::addVertex(const Vec2& v) { vertices.push_back(v); return *this; }
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
