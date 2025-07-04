#include "Collision.h"
#include "Common.h"

#include "raymath.h"

void resolveCollisionElastic(Body& b1, Body& b2)
{
	float distance = Vector3Distance(b1.pos, b2.pos);
	Vector3 r = Vector3Subtract(b2.pos, b1.pos);
	float minDist = b1.radius + b2.radius;

	if (distance <= minDist)
	{
		Vector3 norm = Vector3Normalize(r);
		Vector3 rv = Vector3Subtract(b2.vel, b1.vel);
		float normVel = Vector3DotProduct(rv, norm);

		if (normVel < 0)
		{
			// calculate impulse
			float e = 0.5f; // elasticity [0, 1], controls how much energy is lost per collision
			float j = -(1 + e) * normVel / (1 / b1.mass + 1 / b2.mass);

			Vector3 impulse = Vector3Scale(norm, j);
			b1.vel = Vector3Subtract(b1.vel, Vector3Scale(impulse, 1 / b1.mass));
			b2.vel = Vector3Add(b2.vel, Vector3Scale(impulse, 1 / b2.mass));
		}

		// Separate the bodies immediately
		float overlap = minDist - distance;
		float totalMass = b1.mass + b2.mass;
		float ratio1 = b1.mass / totalMass;
		float ratio2 = b2.mass / totalMass;

		Vector3 separation = Vector3Scale(norm, overlap);
		b1.pos = Vector3Subtract(b1.pos, Vector3Scale(separation, ratio2));
		b2.pos = Vector3Add(b2.pos, Vector3Scale(separation, ratio1));
	}
}

void resolveCollisionCombine(Body& b1, Body& b2) 
{
    float distance = Vector3Distance(b1.pos, b2.pos);
    float minDist = b1.radius + b2.radius;

    if (distance <= minDist) {
        // Determine which body will absorb the other
        Body* larger = &b1;
        Body* smaller = &b2;

        if (b1.mass < b2.mass) {
            larger = &b2;
            smaller = &b1;
        }

        // Calculate conservation of momentum
        Vector3 p1 = Vector3Scale(larger->vel, larger->mass);
        Vector3 p2 = Vector3Scale(smaller->vel, smaller->mass);
        Vector3 totalMomentum = Vector3Add(p1, p2);

        // Update the larger body's properties
        double newMass = larger->mass + smaller->mass;
        double volume = newMass / PLANET_DEFAULT_DENSITY;
        larger->radius = pow(volume / PI, 1.0 / 3.0);
        larger->mass = newMass;
        larger->vel = Vector3Scale(totalMomentum, 1.0 / newMass);

        // Mark smaller body for deletion
        smaller->deleted = true;
    }
}