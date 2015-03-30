#include "ParticleWorld.h"


ParticleWorld::ParticleWorld(unsigned maxContacts, unsigned iterations)
	:mResolver(iterations)
	,mMaxContacts(maxContacts)
{
	mContacts = new ParticleContact[mMaxContacts];
	mCalculateIterations = (iterations == 0);
}

ParticleWorld::~ParticleWorld()
{
	delete[] mContacts;
}

void ParticleWorld::StartFrame()
{
	for (Particles::iterator p = mParticles.begin(); p != mParticles.end(); ++p)
	{
		(*p)->clearAccumluator();
	}
}

unsigned ParticleWorld::GenerateContacts()
{
	unsigned limit = mMaxContacts;
	ParticleContact *nextContact = mContacts;

	for (ContactGenerators::iterator g = mContactGenerators.begin(); g != mContactGenerators.end(); ++g)
	{
		unsigned used = (*g)->AddContact(nextContact, limit);
		limit -= used;
		nextContact += used;

		// Run out of contacts to fill. Missing contacts.
		if (limit <= 0) break;
	}

	// Return number of contacts used
	return mMaxContacts - limit;
}

void ParticleWorld::Integrate(real duration)
{
	for (Particles::iterator p = mParticles.begin(); p != mParticles.end(); ++p)
	{
		// Integrate particle by given duration
		(*p)->integrate(duration);
	}
}

void ParticleWorld::RunPhysics(real duration)
{
	// Apply force generators
	mRegistry.updateForces(duration);
	
	// Integrate objects
	Integrate(duration);

	// Generate contacts
	unsigned usedContacts = GenerateContacts();

	// Process contacts
	if (usedContacts)
	{
		if (mCalculateIterations)
			mResolver.SetIterations(usedContacts * 2);
		mResolver.ResolveContacts(mContacts, usedContacts, duration);
	}
}