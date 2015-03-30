#include "ParticleForceRegistry.h"

void ParticleForceRegistry::add(Particle* particle, ParticleForceGenerator* fg)
{
	ParticleForceRegistration registration;
	registration.particle = particle;
	registration.fg = fg;
	mRegistrations.push_back(registration);
}

void ParticleForceRegistry::remove(Particle* particle, ParticleForceGenerator* fg)
{
	auto i = mRegistrations.begin();
	for (; i != mRegistrations.end(); ++i)
	{
		if (i->fg == fg && i->particle == particle)
		{
			mRegistrations.erase(i);
			break;
		}
	}
}

void ParticleForceRegistry::clear()
{
	mRegistrations.clear();
}

void ParticleForceRegistry::updateForces(real duration)
{
	auto i = mRegistrations.begin();
	for (; i != mRegistrations.end(); ++i)
	{
		i->fg->updateForce(i->particle, duration);
	}
}
