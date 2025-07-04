#include "Body.h"

#include "Common.h"
#include "Random.h"

Body Body::Generate()
{
	Random& rng = Random::getInstance();

	double mass = rng.getDouble(500, 10000);
	double volume = mass / PLANET_DEFAULT_DENSITY;
	double radius = pow(volume / PI, 1.0 / 3.0);

    return {
		{rng.getFloat(-1000, 1000), 0, rng.getFloat(-1000, 1000)},
		{rng.getFloat(-100, 100), rng.getFloat(-5, 5), rng.getFloat(-100, 100)},
		{0, 0, 0},
		mass,
		radius,
		{rng.getUChar(), rng.getUChar(), rng.getUChar(), 255}
	};
}

Body Body::Generate(Vector3 pos, Vector3 vel)
{
	Random& rng = Random::getInstance();

	double mass = rng.getDouble(500, 10000);
	double volume = mass / PLANET_DEFAULT_DENSITY;
	double radius = pow(volume / PI, 1.0 / 3.0);

	return {
		pos,
		vel,
		{0, 0, 0},
		mass,
		radius,
		{rng.getUChar(), rng.getUChar(), rng.getUChar(), 255}
	};
}

Body Body::Generate(Vector3 pos, Vector3 vel, double mass)
{
	Random& rng = Random::getInstance();

	double volume = mass / PLANET_DEFAULT_DENSITY;
	double radius = pow(volume / PI, 1.0 / 3.0);

	return {
		pos,
		vel,
		{0, 0, 0},
		mass,
		radius,
		{rng.getUChar(), rng.getUChar(), rng.getUChar(), 255}
	};
}

Body Body::Generate(Vector3 pos, Vector3 vel, double mass, double radius)
{
	Random& rng = Random::getInstance();

	return {
		pos,
		vel,
		{0, 0, 0},
		mass,
		radius,
		{rng.getUChar(), rng.getUChar(), rng.getUChar(), 255}
	};
}
