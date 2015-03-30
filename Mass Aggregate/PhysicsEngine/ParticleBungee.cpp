#include "ParticleBungee.h"

ParticleBungee::ParticleBungee(Particle* other, real springConstant, real restLength)
	:mOther(other),
	mSpringConstant(springConstant),
	mRestLength(restLength)
{
}

void ParticleBungee::updateForce(Particle* particle, real duration)
{
	// Calculate the vector of the spring
	Vector3 force = particle->getPosition();
	force -= mOther->getPosition();

	// Check if bungee is compressed
	real magnitude = force.Length();
	if (magnitude <= mRestLength) return;

	// Calculate the magnitude of the force
	magnitude = mSpringConstant * (mRestLength - magnitude);

	// Calculate the final force and apply it
	force.Normalize();
	force *= -magnitude;
	particle->addForce(force);
}