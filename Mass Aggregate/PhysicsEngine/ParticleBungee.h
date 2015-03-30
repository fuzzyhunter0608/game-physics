#ifndef PARTICLE_BUNGEE_H
#define PARTICLE_BUNGEE_H

#include "ParticleForceGenerator.h"

class ParticleBungee : public ParticleForceGenerator
{
public:
	ParticleBungee(Particle *other, real springConstant, real restLength);

	virtual void updateForce(Particle *particle, real duration);

private:
	Particle *mOther;
	real mSpringConstant;
	real mRestLength;
};

#endif