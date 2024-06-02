#include "Atom.h"
#include <iostream>
#include "raymath.h"

Atom::Atom(int id, Vector2 pos, const Properties& props)
	: _id(id), _pos(pos), _props(props)
{

}

const int Atom::getID() const
{
	return _id;
}

const Vector2& Atom::getPosition() const
{
	return _pos;
}

const Vector2& Atom::getVelocity() const
{
	return _vel;
}

const Vector2& Atom::getAcceleration() const
{
	return _acc;
}

const Atom::Properties& Atom::getProps() const
{
	return _props;
}

void Atom::setPosition(Vector2 pos)
{
	_pos = pos;
}

void Atom::addBond(std::shared_ptr<Atom> other)
{
	if (_bondedWith.size() >= _maxBonds)
	{
		std::cerr << "Attempted to add a bond beyond the allowable limit!" << std::endl;
		return;
	}

	_bondedWith.insert(other);
}

bool Atom::canFormBond(std::shared_ptr<Atom> other) const
{
	if (_bondedWith.size() >= _maxBonds) return false;
	return !_bondedWith.contains(other);
}

void Atom::applyForce(const Vector2& force)
{
	Vector2 scaledForce = Vector2Scale(force, 1.0f / _props.mass);
	_acc.x += scaledForce.x;
	_acc.y += scaledForce.y;
}

void Atom::tick(float delta)
{
	// update velocity based on any applied forces
	_vel.x += _acc.x * delta;
	_vel.y += _acc.y * delta;

	_vel = Vector2Scale(_vel, 0.9f); // damping (friction)

	// _vel = Vector2Clamp(_vel, { -100, -100 }, { 100, 100 });

	// update position
	_pos.x += _vel.x * delta;
	_pos.y += _vel.y * delta;

	// Reset acceleration for next tick
	_acc.x = 0;
	_acc.y = 0;
}

Atom::Properties::Properties(const Properties& props)
{
	mass = props.mass;
	color = props.color;
	temp = props.temp;
}
