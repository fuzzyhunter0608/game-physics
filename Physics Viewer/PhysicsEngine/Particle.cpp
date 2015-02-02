#include "Particle.h"

void Particle::integrate(real duration)
{
	if (mInverseMass <= 0.0f) return;

	mPosition += mVelocity * duration;

	Vector3 resultingAcc = mAcceleration;
	resultingAcc += forceAccum * mInverseMass;

	mVelocity += resultingAcc * duration;

	mVelocity *= powf(mDamping, duration);

	clearAccumluator();
}

void Particle::clearAccumluator()
{
	forceAccum = Vector3::zero;
}

void Particle::addForce(const Vector3 &force)
{
	forceAccum += force;
}