#ifndef COLLISION_DATA_H
#define COLLISION_DATA_H
#include "Contact.h"

struct CollisionData
{
	Contact *contactArray;
	Contact *contacts;
	int contactsLeft;
	unsigned contactCount;
	real friction;
	real restitution;
	real tolerance;

	bool HasMoreContacts()
	{
		return contactsLeft > 0;
	}

	void Reset(unsigned maxContacts)
	{
		contactsLeft = maxContacts;
		contactCount = 0;
		contacts = contactArray;
	}

	void AddContacts(unsigned count)
	{
		// Reduce the number of contacts remaining, add number used
		contactsLeft -= count;
		contactCount += count;

		// Move the array forward
		contacts += count;
	}
};

#endif