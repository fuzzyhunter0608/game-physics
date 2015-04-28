#ifndef CONTACT_RESOLVER_H
#define CONTACT_RESOLVER_H

#include "Core.h"

class Contact;

class ContactResolver
{
public:
	ContactResolver(unsigned iterations, real velocityEpsilon = (real)0.01, real positionEpsilon = (real)0.01);
	ContactResolver(unsigned velocityIterations, unsigned positionIterations, real velocityEpsilon = (real)0.01, real positionEpsilon = (real)0.01);

	bool IsValid() { return (velocityIterations > 0) && (positionIterations > 0) && (velocityEpsilon >= 0.0f) && (positionEpsilon >= 0.0f); }

	void SetIterations(unsigned iterations);
	void SetIterations(unsigned velocityIterations, unsigned positionIterations);
	void SetEpsilon(real velocityEpsilon, real positionEpsilon);

	void ResolveContacts(Contact *contactArray, unsigned numContacts, real duration);

	unsigned velocityIterationsUsed;
	unsigned positionIterationsUsed;

protected:
	void PrepareContacts(Contact *contactArray, unsigned numContacts, real duration);
	void AdjustVelocites(Contact *contactArray, unsigned numContacts, real duration);
	void AdjustPositions(Contact *contactArray, unsigned numContacts, real duration);

	unsigned velocityIterations;
	unsigned positionIterations;
	real velocityEpsilon;
	real positionEpsilon;

private:
	bool validSettings;
};

#endif