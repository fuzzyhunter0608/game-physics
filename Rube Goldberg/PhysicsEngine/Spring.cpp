#include "Spring.h"
#include "RigidBody.h"

Spring::Spring(const Vector3& localConnectionPt, RigidBody* other, const Vector3& otherConnectionPt, real springConstant, real restLength)
	: connectionPoint(localConnectionPt)
	, otherConnectionPoint(otherConnectionPt)
	, other(other)
	, springConstant(springConstant)
	, restLength(restLength)
{
}

void Spring::UpdateForce(RigidBody* body, real duration)
{
	// Calculate the two ends in world space
	Vector3 lws = body->GetPointInWorldSpace(connectionPoint);
	Vector3 ows = other->GetPointInWorldSpace(otherConnectionPoint);

	// Calculate the vector of the spring
	Vector3 force = lws - ows;

	// Calculate the magnitude of the force
	real magnitude = force.Length();
	magnitude = abs(magnitude - restLength);
	magnitude *= springConstant;

	// Calculate the final force and apply it
	force.Normalize();
	force *= -magnitude;
	body->AddForceAtPoint(force, lws);
}