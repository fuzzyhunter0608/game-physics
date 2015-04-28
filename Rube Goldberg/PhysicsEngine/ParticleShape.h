#ifndef PARTICLE_SHAPE_H
#define PARTICLE_SHAPE_H

#include "Particle.h"
#include "ParticleRod.h"

#include <vector>

class ParticleShape
{
public:
	ParticleShape(int numParticles, int numRods);
	virtual ~ParticleShape();

	virtual void init(const Vector3 &position) = 0;

	vector<Particle*> getParticles() const { return mParticles; }
	vector<ParticleRod*> getRods() const { return mRods; }

protected:
	vector<Particle*> mParticles;
	vector<ParticleRod*> mRods;
};

#endif