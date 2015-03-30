#ifndef PARTICLE_SPRING_H
#define PARTICLE_SPRING_H

#include "ParticleForceGenerator.h"

class ParticleSpring :
	public ParticleForceGenerator
{
public:
	ParticleSpring(Particle *other, real springConstant, real restLength);
	virtual void updateForce(Particle *particle, real duration);

protected:
	Particle *mOther;
	real mSpringConstant;
	real mRestLength;

};

#endif