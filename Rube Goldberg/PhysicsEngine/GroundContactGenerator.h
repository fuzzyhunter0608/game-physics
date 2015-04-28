#ifndef GROUND_CONTACT_GENERATOR_H
#define GROUND_CONTACT_GENERATOR_H

#include "ContactGenerator.h"
#include <vector>
#include "RigidBody.h"
#include "CollisionPrimitive.h"

class GroundContactGenerator : public ContactGenerator
{
public:
	void Add(CollisionBox *box);

	virtual unsigned AddContact(Contact *contact, unsigned limit) const;


private:
	std::vector<CollisionBox*> mBodies;
	CollisionPlane plane;
};

#endif