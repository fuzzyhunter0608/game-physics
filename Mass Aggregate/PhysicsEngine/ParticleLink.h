#ifndef PARTICLE_LINK_H
#define PARTICLE_LINK_H

#include "ParticleContactGenerator.h"

class ParticleLink : public ParticleContactGenerator
{
public:
	Particle* mParticles[2];

	virtual unsigned AddContact(ParticleContact *contact, unsigned limit) const = 0;

protected:
	real CurrentLength() const;
};

#endif