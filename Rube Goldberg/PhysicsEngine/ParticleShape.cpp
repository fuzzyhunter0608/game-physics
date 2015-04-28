#include "ParticleShape.h"

ParticleShape::ParticleShape(int numParticles, int numRods)
{
	mParticles.resize(numParticles);
	mRods.resize(numRods);

	for (vector<ParticleRod*>::iterator iter = mRods.begin(); iter != mRods.end(); ++iter)
	{
		*iter = new ParticleRod();
	}
}

ParticleShape::~ParticleShape()
{
	// Delete Rods
	for (vector<ParticleRod*>::iterator iter = mRods.begin(); iter != mRods.end(); ++iter)
	{
		delete *iter;
	}

	// Delete Particles
	for (vector<Particle*>::iterator iter = mParticles.begin(); iter != mParticles.end(); ++iter)
	{
		delete *iter;
	}
}
