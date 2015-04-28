#include "ParticleCable.h"

unsigned ParticleCable::AddContact(ParticleContact *contact, unsigned limit) const
{
	// Find length of cable
	real length = CurrentLength();

	// Check if overextended
	if (length < mMaxLength)
	{
		return 0;
	}

	// Otherwise, return contact
	contact->mParticles[0] = mParticles[0];
	contact->mParticles[1] = mParticles[1];

	// Calculate normal
	Vector3 normal = mParticles[1]->getPosition() - mParticles[0]->getPosition();
	normal.Normalize();
	contact->mNormal = normal;

	contact->mPenetration = length - mMaxLength;
	contact->mRestitution = mRestitution;

	return 1;
}