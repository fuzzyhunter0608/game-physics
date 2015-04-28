#ifndef GRAVITY_H
#define GRAVITY_H

#include "ForceGenerator.h"
#include "Vector3.h"

class Gravity : public ForceGenerator
{
public:
	Gravity(const Vector3 &gravity);

	virtual void UpdateForce(RigidBody *body, real duration);
private:
	Vector3 gravity;
};

#endif