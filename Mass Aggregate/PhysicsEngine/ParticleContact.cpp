#include "ParticleContact.h"

void ParticleContact::Resolve(real duration)
{
	ResolveVelocity(duration);
	ResolveInterpenetration(duration);
}

real ParticleContact::CalculateSeparatingVelocity() const
{
	Vector3 relativeVelocity = mParticles[0]->getVelocity();
	if (mParticles[1])
	{
		relativeVelocity -= mParticles[1]->getVelocity();
	}

	return Vector3::Dot(relativeVelocity, mNormal);
}

void ParticleContact::ResolveVelocity(real duration)
{
	// Find velocity in direction of contact
	real separatingVelocity = CalculateSeparatingVelocity();

	// Check if needs to be resolved
	if (separatingVelocity > 0)
	{
		// Contact is separating or stationary
		return;
	}

	// Calculate new separating velocity
	real newSepVelocity = -separatingVelocity * mRestitution;

	// Check velocity buildup due to acceleration only
	Vector3 accCausedVelocity = mParticles[0]->getAcceleration();
	if (mParticles[1])
	{
		accCausedVelocity -= mParticles[1]->getAcceleration();
	}
	real accCausedSepVelocity = Vector3::Dot(accCausedVelocity, mNormal) * duration;

	// If closing velocity due to acceleration buildup, remove from new separating velocity
	if (accCausedSepVelocity < 0)
	{
		newSepVelocity += mRestitution * accCausedSepVelocity;

		// Clamp to zero
		if (newSepVelocity < 0) newSepVelocity = 0;
	}

	real deltaVelocity = newSepVelocity - separatingVelocity;

	// Apply change in velocity to each particle in proportion to inverse mass
	real totalInverseMass = mParticles[0]->getInverseMass();
	if (mParticles[1])
	{
		totalInverseMass += mParticles[1]->getInverseMass();
	}

	// If all particles have infinite mass, impulses have no effect
	if (totalInverseMass <= 0) return;

	// Calculate the impulse to apply
	real impulse = deltaVelocity / totalInverseMass;

	// Find amount of impulse per unit of inverse mass;
	Vector3 impulsePerIMass = mNormal * impulse;

	// Apply impulses, they are applied in direction of contact, proportional to inverse mass
	mParticles[0]->setVelocity(mParticles[0]->getVelocity() + impulsePerIMass * mParticles[0]->getInverseMass());
	if (mParticles[1])
	{
		mParticles[1]->setVelocity(mParticles[1]->getVelocity() + impulsePerIMass * -mParticles[1]->getInverseMass());
	}
}

void ParticleContact::ResolveInterpenetration(real duration)
{
	// If no penetration, skip
	if (mPenetration <= 0) return;

	// Movement of each object is based on inverse mass; total that
	real totalInverseMass = mParticles[0]->getInverseMass();
	if (mParticles[1])
	{
		totalInverseMass += mParticles[1]->getInverseMass();
	}

	// If all particles have infinite mass, do nothing
	if (totalInverseMass <= 0) return;

	// Find amount of penetration resolution per unit of inverse mass
	Vector3 movePerIMass = mNormal * (mPenetration / totalInverseMass);

	// Calculate movement amounts
	mParticleMovement[0] = movePerIMass * mParticles[0]->getInverseMass();
	if (mParticles[1])
	{
		mParticleMovement[1] = movePerIMass * -mParticles[1]->getInverseMass();
	}
	else
	{
		mParticleMovement[1] = Vector3::zero;
	}

	// Apply penetration resolution
	mParticles[0]->setPosition(mParticles[0]->getPosition() + mParticleMovement[0]);
	if (mParticles[1])
	{
		mParticles[1]->setPosition(mParticles[1]->getPosition() + mParticleMovement[1]);
	}
}