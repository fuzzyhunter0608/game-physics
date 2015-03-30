#ifndef PARTICLE_CONTACT_GENERATOR_H
#define PARTICLE_CONTACT_GENERATOR_H

#include "ParticleContact.h"

class ParticleContactGenerator
{
public:
	virtual unsigned AddContact(ParticleContact *contact, unsigned limit) const = 0;
};

#endif