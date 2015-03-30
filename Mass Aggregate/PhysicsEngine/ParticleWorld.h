#ifndef PARTICLE_WORLD_H
#define PARTICLE_WORLD_H

#include <vector>

#include "Particle.h"
#include "ParticleContactGenerator.h"
#include "ParticleContactResolver.h"
#include "ParticleForceRegistry.h"

class ParticleWorld
{
public:
	typedef std::vector<Particle*> Particles;
	typedef std::vector<ParticleContactGenerator*> ContactGenerators;

	ParticleWorld(unsigned maxContacts, unsigned iterations = 0);
	~ParticleWorld();

	Particles& GetParticles() { return mParticles; }
	ContactGenerators& GetContactGenerators() { return mContactGenerators; }
	ParticleForceRegistry& GetForceRegistry() { return mRegistry; }

	void StartFrame();
	unsigned GenerateContacts();
	void Integrate(real duration);
	void RunPhysics(real duration);

protected:
	Particles mParticles;
	ParticleForceRegistry mRegistry;
	ParticleContactResolver mResolver;
	ContactGenerators mContactGenerators;
	ParticleContact *mContacts;
	unsigned mMaxContacts;
	bool mCalculateIterations;
};

#endif