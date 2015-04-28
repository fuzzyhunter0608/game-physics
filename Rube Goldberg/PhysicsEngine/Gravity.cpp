#include "Gravity.h"
#include "RigidBody.h"

Gravity::Gravity(const Vector3& gravity)
	:gravity(gravity)
{
}

void Gravity::UpdateForce(RigidBody* body, real duration)
{
	// Check that we do not have infinite maass
	if (!body->HasFiniteMass()) return;

	// Apply the mass-scaled force to the body
	auto test = gravity * body->GetMass();
	body->AddForce(gravity * body->GetMass());
}