#ifndef FORCE_GENERATOR_H
#define FORCE_GENERATOR_H

#include "Core.h"

class RigidBody;

class ForceGenerator
{
public:
	virtual ~ForceGenerator() {}

	virtual void UpdateForce(RigidBody *body, real duration) = 0;
};

#endif
