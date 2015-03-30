#include "ParticleFactory.h"

Particle* ParticleFactory::GetParticle(real mass, Vector3 position, Vector3 velocity, real damping)
{
	Particle* newParticle = new Particle();

	newParticle->setMass(mass);
	newParticle->setPosition(position);
	newParticle->setVelocity(velocity);
	newParticle->setDamping(damping);

	return newParticle;
}
