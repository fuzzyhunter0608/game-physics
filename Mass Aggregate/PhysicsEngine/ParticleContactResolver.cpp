#include "ParticleContactResolver.h"

ParticleContactResolver::ParticleContactResolver(const unsigned iterations)
	:mIterations(iterations)
{
}

void ParticleContactResolver::ResolveContacts(ParticleContact* contactArray, unsigned numContacts, real duration)
{
	unsigned i;

	mIterationsUsed = 0;
	while (mIterationsUsed < mIterations)
	{
		// Find contact with largest closing velocity
		real max = REAL_MAX;
		unsigned maxIndex = numContacts;
		for (i = 0; i < numContacts; ++i)
		{
			real sepVel = contactArray[i].CalculateSeparatingVelocity();
			if (sepVel < max && (sepVel < 0 || contactArray[i].mPenetration > 0))
			{
				max = sepVel;
				maxIndex = i;
			}
		}

		// Is there anything worth resolving?
		if (maxIndex == numContacts)
		{
			break;
		}

		// Resolve this contact
		contactArray[maxIndex].Resolve(duration);

		// Update interpenetrations for all particles
		Vector3 *move = contactArray[maxIndex].mParticleMovement;
		for (i = 0; i < numContacts; i++)
		{
			if (contactArray[i].mParticles[0] == contactArray[maxIndex].mParticles[0])
			{
				contactArray[i].mPenetration -= Vector3::Dot(move[0], contactArray[i].mNormal);
			}
			else if (contactArray[i].mParticles[0] == contactArray[maxIndex].mParticles[1])
			{
				contactArray[i].mPenetration -= Vector3::Dot(move[1], contactArray[i].mNormal);
			}

			if (contactArray[i].mParticles[1])
			{
				if (contactArray[i].mParticles[1] == contactArray[maxIndex].mParticles[0])
				{
					contactArray[i].mPenetration += Vector3::Dot(move[0], contactArray[i].mNormal);
				}
				else if (contactArray[i].mParticles[1] == contactArray[maxIndex].mParticles[1])
				{
					contactArray[i].mPenetration += Vector3::Dot(move[1], contactArray[i].mNormal);
				}
			}
		}

		++mIterationsUsed;
	}
}