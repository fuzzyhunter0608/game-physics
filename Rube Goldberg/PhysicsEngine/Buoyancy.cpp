#include "Buoyancy.h"
#include "RigidBody.h"

Buoyancy::Buoyancy(const Vector3& cOfB, real maxDepth, real volume, real waterHeight, real liquidDensity)
	: maxDepth(maxDepth)
	, volume(volume)
	, waterHeight(waterHeight)
	, liquidDensity(liquidDensity)
	, centerOfBuoyancy(cOfB)
{
}

void Buoyancy::UpdateFoce(RigidBody* body, real duration)
{
	// Calculate the submersion depth
	Vector3 pointInWorld = body->GetPointInWorldSpace(centerOfBuoyancy);
	real depth = pointInWorld.y;

	// Check if we're out of the water
	if (depth >= waterHeight - maxDepth) return;
	Vector3 force(0, 0, 0);

	// Check if we're at maximum depth
	if (depth <= waterHeight - maxDepth)
	{
		force.y = liquidDensity * volume;
		body->AddForceAtBodyPoint(force, centerOfBuoyancy);
		return;
	}

	// Otherwise we are partly submerged
	force.y = liquidDensity * volume * (depth - maxDepth - waterHeight) / 2 * maxDepth;
	body->AddForceAtBodyPoint(force, centerOfBuoyancy);
}