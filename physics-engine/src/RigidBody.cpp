#include "RigidBody.h"

using namespace Physics;

RigidBody::RigidBody() {
	setMass(1.0);
}

void RigidBody::resolve() {
	computeInertia();
	computeCenterOfMass();

	resolved = true;
}

RigidBody& RigidBody::setMass(double m) {
	mass = m;
	invMass = (mass > 0) ? 1.0 / mass : 0.0;
	return *this;
}

void RigidBody::computeCenterOfMass() {
	if (vertices.empty()) {
		centerOfMass = Vec2(0, 0);
		return;
	}
	Vec2 sum(0, 0);
	for (const Vec2& v : vertices) {
		sum += v;
	}
	centerOfMass = sum / static_cast<double>(vertices.size());
}

void RigidBody::computeInertia() {
	// Approximate inertia for box or circle
	if (isCircle) {
		// Assume radius = scale.x (uniform scale)
		double r = transform.scale.x;
		inertia = 0.5 * mass * r * r;
	} else if (vertices.size() >= 2) {
		// Box: I = 1/12 * m * (w^2 + h^2)
		double w = transform.scale.x;
		double h = transform.scale.y;
		inertia = (1.0 / 12.0) * mass * (w * w + h * h);
	} else {
		inertia = 1.0;
	}
	invInertia = (inertia > 0) ? 1.0 / inertia : 0.0;
}

RigidBody& RigidBody::addVertices(const std::vector<Vec2>& vs) {
	vertices.insert(vertices.end(), vs.begin(), vs.end());
	return *this;
}
RigidBody& RigidBody::setVertices(const std::vector<Vec2>& vs) {
	vertices = vs;
	return *this;
}
RigidBody& RigidBody::setVertices(const double* vs, size_t count) {
	vertices.clear();
	for (size_t i = 0; i < count; i += 2) {
		vertices.emplace_back(vs[i], vs[i + 1]);
	}
	return *this;
}
RigidBody& RigidBody::setShape(Shape shape) {
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

void RigidBody::applyForce(const Vec2& force) {
	if (!kinematic) return;
	acc += force * invMass;
}

void RigidBody::applyForce(const Vec2& force, const Vec2& worldPoint) {
	if (!kinematic) return;
	acc += force * invMass;
	Vec2 r = worldPoint - transform.position;
	torque += r.cross(force);
}

void RigidBody::applyTorque(double t) {
	if (!kinematic) return;
	torque += t;
}

void RigidBody::impulse(const Vec2& impulse, const Vec2& worldPoint) {
	if (!kinematic) return;

	lastImpulse = impulse;
	lastContactPoint = worldPoint;

	vel += impulse * invMass;
	// Angular impulse: r x J
	Vec2 r = worldPoint - transform.position;
	angularVelocity += invInertia * r.cross(impulse);
}

void RigidBody::update(double dt) {
	if (!resolved) resolve();

	if (!kinematic) return;
	vel += acc * dt;
	transform.position += vel * dt;

	// Angular motion
	angularAcc = torque * invInertia;
	angularVelocity += angularAcc * dt;
	transform.rotation += angularVelocity * dt;

	// Reset for next step
	acc = Vec2(0, 0);
	torque = 0.0;
}
