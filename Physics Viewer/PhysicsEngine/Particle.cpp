#include "Particle.h"

#include <iostream>

void Particle::integrate(real duration)
{
	//real newDuration = duration * 60 * 60 * 60 * 24;

	if (mInverseMass <= 0.0f) return;

	mPosition.AddScaledVector(mVelocity, duration);

	Vector3 resultingAcc = mAcceleration;
	resultingAcc.AddScaledVector(forceAccum, mInverseMass);

	mVelocity.AddScaledVector(resultingAcc, duration);

	mVelocity *= powf(mDamping, duration);

	std::cout << mVelocity.Length() << endl;

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