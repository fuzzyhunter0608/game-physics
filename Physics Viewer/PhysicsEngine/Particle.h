#pragma once

#include "Math.h"

class Particle
{
public:
	// Accessors
	void setPosition(const Vector3 &position) { mPosition = position; }
	void setPosition(const real x, const real y, const real z) { mPosition = Vector3(x, y, z); }
	Vector3 getPosition() const { return mPosition; }
	void setVelocity(const Vector3 &velocity) { mVelocity = velocity; }
	void setVelocity(const real x, const real y, const real z) { mVelocity = Vector3(x, y, z); }
	Vector3 getVelocity() const { return mVelocity; }
	void setAcceleration(const Vector3 &acceleration) { mAcceleration = acceleration; }
	void setAcceleration(const real x, const real y, const real z) { mAcceleration = Vector3(x, y, z); }
	Vector3 getAcceleration() const { return mAcceleration; }

	void setDamping(const real damping) { mDamping = damping; }
	real getDamping() const { return mDamping; }

	void setMass(const real mass) { mInverseMass = (real)1.0 / mass; }
	real getMass() const { return 1 / mInverseMass; }
	void setInverseMass(const real mass) { mInverseMass = mass; }
	real getInverseMass(const real mass) { return mInverseMass; }

	// Functions
	void integrate(real duration);
	void clearAccumluator();
	void addForce(const Vector3 &force);

protected:
	Vector3 mPosition;
	Vector3 mVelocity;
	Vector3 mAcceleration;
	Vector3 forceAccum;

	real mDamping;
	real mInverseMass;
};