#pragma once

#include "ParticleForceGenerator.h"

class ParticleGravity : public ParticleForceGenerator
{
public:
	ParticleGravity(Particle* attractionParticle);

	virtual void updateForce(Particle* particle, real duration);

protected:
	Particle* mpAttractionParticle;
};

