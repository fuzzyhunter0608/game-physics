#include "Particle.h"

#include <iostream>

void Particle::integrate(real duration)
{
	//real newDuration = duration * 60 * 60 * 60 * 24;

	if (mInverseMass <= 0.0f) return;

	//Timestep
	int numSteps = 128;
	real timestep = duration / numSteps;

	for (int i = 1; i <= numSteps; ++i)
	{
		mPosition.AddScaledVector(mVelocity, timestep);

		Vector3 resultingAcc = mAcceleration;
		resultingAcc.AddScaledVector(forceAccum, mInverseMass);

		mVelocity.AddScaledVector(resultingAcc, timestep);

		mVelocity *= powf(mDamping, timestep);
	}

	//std::cout << mVelocity.Length() << endl;

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