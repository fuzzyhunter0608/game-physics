#ifndef SPRING_H
#define SPRING_H

#include "ForceGenerator.h"
#include "Vector3.h"

class Spring : public ForceGenerator
{
public:
	Spring(const Vector3 &localConnectionPt, RigidBody *other, const Vector3 &otherConnectionPt, real springConstant, real restLength);

	virtual void UpdateForce(RigidBody *body, real duration);

private:
	Vector3 connectionPoint;
	Vector3 otherConnectionPoint;
	RigidBody *other;
	real springConstant;
	real restLength;
};

#endif