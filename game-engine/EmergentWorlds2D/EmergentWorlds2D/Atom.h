#pragma once
#include "raylib.h"
#include <unordered_set>
#include <memory>

class Atom
{
public:
	struct Properties 
	{
		Properties() = default;
		Properties(const Properties& props); // copy constructor

		float mass = 1.0f; // mass, units ???
		float temp = 275.0f; // temp, Kelvin
		Color color = WHITE;
	};

public:
	Atom() = delete; // no default constructor
	Atom(int id, Vector2 pos, const Properties& props);

	const int getID() const;
	const Vector2& getPosition() const;
	const Vector2& getVelocity() const;
	const Vector2& getAcceleration() const;
	const Properties& getProps() const;

	void setPosition(Vector2 pos);

	// bonds
	void addBond(std::shared_ptr<Atom> other);
	bool canFormBond(std::shared_ptr<Atom> other) const;

	// physics methods
	void applyForce(const Vector2& force);

	void tick(float delta);
private:
	int _id;
	Vector2 _pos;
	Vector2 _vel{0, 0}, _acc{0, 0};
	int _maxBonds = 6;
	Properties _props;
	std::unordered_set<std::shared_ptr<Atom>> _bondedWith;
};
