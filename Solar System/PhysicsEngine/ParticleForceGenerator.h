#pragma once

#include "Particle.h"

class ParticleForceGenerator
{
public:
	virtual void updateForce(Particle* particle, real duration) = 0;
};
