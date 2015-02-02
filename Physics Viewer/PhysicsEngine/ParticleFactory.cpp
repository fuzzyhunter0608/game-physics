#include "ParticleFactory.h"

Particle* ParticleFactory::GetParticle(Planets planet)
{
	Particle* newParticle = new Particle();

	switch(planet)
	{
	case SUN:
		newParticle->setMass(333000 * 5000);
		newParticle->setPosition(Vector3::zero);
		break;
	case EARTH:
		newParticle->setMass(5000);
		newParticle->setPosition(18.4, 0, 0);
		newParticle->setVelocity(0, 0.1, 0);
		newParticle->setDamping(0.99);
		break;
	}

	return newParticle;
}
