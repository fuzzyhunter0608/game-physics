#ifndef PARTICLE_GRAVITY_EARTH_H
#define PARTICLE_GRAVITY_EARTH_H

#include "ParticleForceGenerator.h"

class ParticleGravityEarth : public ParticleForceGenerator
{
public:
	ParticleGravityEarth(const Vector3 &gravity);

	virtual void updateForce(Particle* particle, real duration);

private:
	Vector3 mGravity;
};

#endif