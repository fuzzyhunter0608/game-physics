#include "GroundContactGenerator.h"
#include "Contact.h"
#include "CollisionDetector.h"

void GroundContactGenerator::Add(CollisionBox* box)
{
	mBodies.push_back(box);
}

unsigned GroundContactGenerator::AddContact(Contact* contact, unsigned limit) const
{
	unsigned count = 0;

	for (auto i = mBodies.begin(); i != mBodies.end(); ++i)
	{
		//if (CollisionDetector::BoxAndHalfSpace((*i), plane, contact))

		if (count >= limit) return count;
	}

	return count;
}
