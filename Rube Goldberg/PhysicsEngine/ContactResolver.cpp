#include "ContactResolver.h"

#include "Contact.h"

ContactResolver::ContactResolver(unsigned iterations, real velocityEpsilon, real positionEpsilon)
{
	SetIterations(iterations);
	SetEpsilon(velocityEpsilon, positionEpsilon);
}

ContactResolver::ContactResolver(unsigned velocityIterations, unsigned positionIterations, real velocityEpsilon, real positionEpsilon)
{
	SetIterations(velocityIterations, positionIterations);
	SetEpsilon(velocityEpsilon, positionEpsilon);
}

void ContactResolver::SetIterations(unsigned iterations)
{
	SetIterations(iterations, iterations);
}

void ContactResolver::SetIterations(unsigned velocityIterations, unsigned positionIterations)
{
	this->velocityIterations = velocityIterations;
	this->positionIterations = positionIterations;
}

void ContactResolver::SetEpsilon(real velocityEpsilon, real positionEpsilon)
{
	this->velocityEpsilon = velocityEpsilon;
	this->positionEpsilon = positionEpsilon;
}

void ContactResolver::ResolveContacts(Contact* contactArray, unsigned numContacts, real duration)
{
	// Make sure we have something to do
	if (numContacts == 0) return;
	if (!IsValid()) return;

	// Prepare the contacts for procession
	PrepareContacts(contactArray, numContacts, duration);

	// Resolve the interpenetrations problems with the contacts
	AdjustPositions(contactArray, numContacts, duration);

	// Resolve the velocity problems with the contacts
	AdjustVelocites(contactArray, numContacts, duration);
}

void ContactResolver::PrepareContacts(Contact* contactArray, unsigned numContacts, real duration)
{
	// Generate contact velocity and axis information
	Contact *lastContact = contactArray + numContacts;
	for (Contact *contact = contactArray; contact < lastContact; contact++)
	{
		// Calculate the internal contact data (inertia, basis, etc)
		contact->CalculateInternals(duration);
	}
}

void ContactResolver::AdjustVelocites(Contact* contactArray, unsigned numContacts, real duration)
{
	Vector3 velocityChange[2], rotationChange[2];
	Vector3 deltaVel;

	// iteratively handle impacts in order of severity
	velocityIterationsUsed = 0;
	while (velocityIterationsUsed < velocityIterations)
	{
		// Find contact with maximum magnitude of probable velocity change
		real max = velocityEpsilon;
		unsigned index = numContacts;
		for (unsigned i = 0; i < numContacts; ++i)
		{
			if (contactArray[i].desiredDeltaVelocity > max)
			{
				max = contactArray[i].desiredDeltaVelocity;
				index = i;
			}
		}

		if (index == numContacts) break;

		// Match the awake state at the contact
		contactArray[index].MatchAwakeState();

		// Do the resolution on the contact that came out top
		contactArray[index].ApplyVelocityChange(velocityChange, rotationChange);

		// With the change in velocity of the two bodies, the update of contact velocities means that some of the relative closing velocities need recomputing
		for (unsigned i = 0; i < numContacts; ++i)
		{
			// Check each body in the contact
			for (unsigned b = 0; b < 2; ++b)
			{
				if (contactArray[i].body[b])
				{
					// Check for a match with each body in the newly resolved contact
					for (unsigned d = 0; d < 2; d++)
					{
						if (contactArray[i].body[b] == contactArray[index].body[d])
						{
							deltaVel = velocityChange[d] + Vector3::Cross(rotationChange[d], contactArray[i].relativeContactPosition[b]);

							// The sign of the change is negative if we're dealing with the second body in a contact
							contactArray[i].contactVelocity += contactArray[i].contactToWorld.transformTranspose(deltaVel) * (b ? -1 : 1);
							contactArray[i].CalculateDesiredDeltaVelocity(duration);
						}
					}
				}
			}
		}

		velocityIterationsUsed++;
	}
}

void ContactResolver::AdjustPositions(Contact* contactArray, unsigned numContacts, real duration)
{
	unsigned i, index;
	Vector3 linearChange[2], angularChange[2];
	real max;
	Vector3 deltaPosition;

	// iteratively resolve interpenetrations in order of severity
	positionIterationsUsed = 0;
	while (positionIterationsUsed < positionIterations)
	{
		// Find biggest penetration
		max = positionEpsilon;
		index = numContacts;
		for (i = 0; i < numContacts; ++i)
		{
			if (contactArray[i].penetration > max)
			{
				max = contactArray[i].penetration;
				index = i;
			}
		}
		if (index = numContacts) break;

		// Match the awake state at the contact
		contactArray[index].MatchAwakeState();

		// Resolve the penetration
		contactArray[index].ApplyPositionChange(linearChange, angularChange, max);

		// Again this action may have changed the penetration of other bodies, so we update contacts
		for (i = 0; i < numContacts; i++)
		{
			// Check each body in the contact
			for (unsigned b = 0; b < 2; b++) if (contactArray[i].body[b])
			{
				// Check for a match with each body in the newly
				// resolved contact
				for (unsigned d = 0; d < 2; d++)
				{
					if (contactArray[i].body[b] == contactArray[index].body[d])
					{
						deltaPosition = linearChange[d] + Vector3::Cross(angularChange[d], contactArray[i].relativeContactPosition[b]);

						// The sign of the change is positive if we're
						// dealing with the second body in a contact
						// and negative otherwise (because we're
						// subtracting the resolution)..
						contactArray[i].penetration += Vector3::Dot(deltaPosition, contactArray[i].contactNormal) * (b ? 1 : -1);
					}
				}
			}
		}
		positionIterationsUsed++;
	}
}