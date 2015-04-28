#ifndef JOINT_H
#define JOINT_H

#include "ContactGenerator.h"
#include "RigidBody.h"

class Joint : public ContactGenerator
{
public:
	RigidBody *body[2];
	Vector3 position[2];
	real error;

	void Set(RigidBody *a, const Vector3 &aPos, RigidBody *b, const Vector3 &bPos, real error);

	unsigned AddContact(Contact *contact, unsigned limit) const;
};

#endif