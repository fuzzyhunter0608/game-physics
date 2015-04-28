#ifndef BUOYANCY_H
#define BUOYANCY_H

#include "ForceGenerator.h"
#include "Vector3.h"

class Buoyancy : public ForceGenerator
{
public:
	Buoyancy(const Vector3 &cOfB, real maxDepth, real volume, real waterHeight, real liquidDensity = 1000.0f);

	virtual void UpdateFoce(RigidBody *body, real duration);

private:
	real maxDepth;
	real volume;
	real waterHeight;
	real liquidDensity;
	Vector3 centerOfBuoyancy;
};

#endif