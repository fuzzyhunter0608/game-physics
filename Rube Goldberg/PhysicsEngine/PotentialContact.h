#ifndef POTENTIAL_CONTACT_H
#define POTENTIAL_CONTACT_H

#include "RigidBody.h"

struct PotentialContact
{
	RigidBody *body[2];
};

#endif