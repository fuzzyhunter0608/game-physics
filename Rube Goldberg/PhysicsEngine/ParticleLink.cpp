#include "ParticleLink.h"

real ParticleLink::CurrentLength() const
{
	Vector3 relativePos = mParticles[0]->getPosition() - mParticles[1]->getPosition();
	return relativePos.Length();
}