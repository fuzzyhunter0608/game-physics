#include "ParticleSpring.h"

#include <cmath>

ParticleSpring::ParticleSpring(Particle *other, real springConstant, real restLength)
	:mOther(other),
	mSpringConstant(springConstant),
	mRestLength(restLength)
{
}

void ParticleSpring::updateForce(Particle *particle, real duration)
{
	// Calculate the vector of the spring
	Vector3 force;
	force = particle->getPosition();
	force -= mOther->getPosition();

	// Calculate the magnitude of the force
	real magnitude = force.Length();
	magnitude = abs(magnitude - mRestLength);
	magnitude *= mSpringConstant;

	// Calculate the final force and apply it
	force.Normalize();
	force *= -magnitude;
	particle->addForce(force);
}