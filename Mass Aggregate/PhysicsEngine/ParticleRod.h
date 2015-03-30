#ifndef PARTICLE_ROD_H
#define PARTICLE_ROD_H

#include "ParticleLink.h"

class ParticleRod : public ParticleLink
{
public:
	virtual unsigned AddContact(ParticleContact *contact, unsigned limit) const;
	real mLength;
};

#endif