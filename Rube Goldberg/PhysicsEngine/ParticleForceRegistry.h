#pragma once

#include <vector>

#include "ParticleForceGenerator.h"

struct ParticleForceRegistration
{
	Particle* particle;
	ParticleForceGenerator* fg;
};

typedef vector<ParticleForceRegistration> ParticleRegistry;

class ParticleForceRegistry
{
public:
	void add(Particle* particle, ParticleForceGenerator* fg);
	void remove(Particle* particle, ParticleForceGenerator* fg);
	void clear();
	
	void updateForces(real duration);

protected:
	ParticleRegistry mRegistrations;
};

