#ifndef PARTICLE_GROUND_H
#define PARTICLE_GROUND_H

#include "ParticleContactGenerator.h"
#include "ParticleWorld.h"

class ParticleGround : public ParticleContactGenerator
{
public:
	void init(ParticleWorld::Particles *particles);
	virtual unsigned AddContact(ParticleContact *contact, unsigned limit) const;

private:
	ParticleWorld::Particles *mParticles;
};

#endif