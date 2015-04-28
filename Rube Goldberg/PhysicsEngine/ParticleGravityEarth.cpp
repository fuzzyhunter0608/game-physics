#include "ParticleGravityEarth.h"

ParticleGravityEarth::ParticleGravityEarth(const Vector3 &gravity)
	:mGravity(gravity)
{
}

void ParticleGravityEarth::updateForce(Particle* particle, real duration)
{
	// Check that we do not have infinite mass
	if (!particle->hasFiniteMass())
	{
		return;
	}

	// Apply mass-scaled force to particle
	particle->addForce(mGravity * particle->getMass());
}