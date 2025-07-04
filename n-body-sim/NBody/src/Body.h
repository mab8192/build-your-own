#pragma once

#include "raylib.h"

struct Body
{
	Vector3 pos;
	Vector3 vel;
	Vector3 acc;
	double mass;
	double radius;
	Color color = WHITE;
	bool deleted = false;

	static Body Generate();
	static Body Generate(Vector3 pos, Vector3 vel);
	static Body Generate(Vector3 pos, Vector3 vel, double mass);
	static Body Generate(Vector3 pos, Vector3 vel, double mass, double radius);
};
