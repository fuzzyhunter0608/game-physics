#ifndef PARTICLE_CONTACT_RESOLVER_H
#define PARTICLE_CONTACT_RESOLVER_H

#include "ParticleContact.h"

class ParticleContactResolver
{
public:
	ParticleContactResolver(const unsigned iterations);

	void SetIterations(const unsigned iterations) { mIterations = iterations; }
	void ResolveContacts(ParticleContact* contactArray, unsigned numContacts, real duration);

protected:
	unsigned mIterations;
	unsigned mIterationsUsed;
};

#endif