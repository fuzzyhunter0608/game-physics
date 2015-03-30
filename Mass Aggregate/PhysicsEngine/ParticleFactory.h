#pragma once

#include "Particle.h"


class ParticleFactory
{
public:
	Particle* GetParticle(real mass, Vector3 position, Vector3 velocity, real damping);
};

