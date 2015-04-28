#include "ParticleRod.h"

unsigned ParticleRod::AddContact(ParticleContact *contact, unsigned limit) const
{
	// Find length of rod
	real currentLength = CurrentLength();

	// Check if overextended
	if (currentLength == mLength)
	{
		return 0;
	}

	// Otherwise, return contact
	contact->mParticles[0] = mParticles[0];
	contact->mParticles[1] = mParticles[1];

	// Calculate normal
	Vector3 normal = mParticles[1]->getPosition() - mParticles[0]->getPosition();
	normal.Normalize();

	// Contact normal depends on extending or compressing
	if (currentLength > mLength)
	{
		contact->mNormal = normal;
		contact->mPenetration = currentLength - mLength;
	}
	else
	{
		contact->mNormal = normal * -1;
		contact->mPenetration = mLength - currentLength;
	}

	// Always use 0 restitution - no bounciness
	contact->mRestitution = 0;

	return 1;
}