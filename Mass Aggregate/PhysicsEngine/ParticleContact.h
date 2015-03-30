#ifndef PARTICLE_CONTACT_H
#define PARTICLE_CONTACT_H

#include "Particle.h"

class ParticleContact
{
	friend class ParticleContactResolver;

public:
	Particle* mParticles[2];
	real mRestitution;
	Vector3 mNormal;
	real mPenetration;
	Vector3 mParticleMovement[2];

protected:
	void Resolve(real duration);
	real CalculateSeparatingVelocity() const;

private:
	void ResolveVelocity(real duration);
	void ResolveInterpenetration(real duration);
};

#endif