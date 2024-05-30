#include "Atom.h"
#include <iostream>
#include "raymath.h"

Atom::Atom(Vector2 pos, Color col)
	: _pos(pos), _color(col) 
{
	_vel = Vector2();
	_acc = Vector2();
	_mass = 1.0f;
	_temp = 273.25f;
}

const Vector2& Atom::getPosition() const
{
	// TODO: insert return statement here
	return _pos;
}

const Vector2& Atom::getVelocity() const
{
	// TODO: insert return statement here
	return _vel;
}

const Vector2& Atom::getAcceleration() const
{
	// TODO: insert return statement here
	return _acc;
}

const Color& Atom::getColor() const
{
	// TODO: insert return statement here
	return _color;
}

const int Atom::getMaxBonds() const
{
	return _maxBonds;
}

bool Atom::canFormBond() const
{
	return _numBonds < _maxBonds;
}

void Atom::addBond()
{
	_numBonds++;
}

void Atom::breakBond()
{
	_numBonds--;
}

void Atom::setColor(Color color)
{
	_color = color;
}

void Atom::applyForce(const Vector2& force)
{
	Vector2 scaledForce = Vector2Scale(force, 1.0f / _mass);
	_acc.x += scaledForce.x;
	_acc.y += scaledForce.y;
}

void Atom::tick(double delta)
{
	// update velocity based on any applied forces
	_vel.x += _acc.x * delta * 0.9f;
	_vel.y += _acc.y * delta * 0.9f;

	// Apply damping to velocity
	_vel = Vector2Scale(_vel, 0.8f);

	// update position
	_pos.x += _vel.x * delta;
	_pos.y += _vel.y * delta;

	// Reset acceleration for next tick
	_acc.x = 0;
	_acc.y = 0;
}

