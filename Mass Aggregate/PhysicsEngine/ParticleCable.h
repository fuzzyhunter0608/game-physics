#pragma once
#include "ParticleLink.h"
class ParticleCable :
	public ParticleLink
{
public:
	real mMaxLength;
	real mRestitution;

	virtual unsigned AddContact(ParticleContact *contact, unsigned limit) const;
};

