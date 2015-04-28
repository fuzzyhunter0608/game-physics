#include "ParticleGround.h"

void ParticleGround::init(ParticleWorld::Particles *particles)
{
	mParticles = particles;
}

unsigned ParticleGround::AddContact(ParticleContact *contact, unsigned limit) const
{
	unsigned count = 0;

	for (ParticleWorld::Particles::iterator p = mParticles->begin(); p != mParticles->end(); ++p)
	{
		real y = (*p)->getPosition().y;
		if (y < 0.0)
		{
			contact->mNormal = Vector3(0, 1, 0);
			contact->mParticles[0] = *p;
			contact->mParticles[1] = NULL;
			contact->mPenetration = -y;
			contact->mRestitution = 0.2f;
			contact++;
			count++;
		}

		if (count >= limit) return count;
	}

	return count;
}