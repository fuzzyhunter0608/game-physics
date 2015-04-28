#include "World.h"
#include "Contact.h"

World::World(unsigned maxContacts, unsigned iterations)
	: resolver(iterations)
	, maxContacts(maxContacts)
{
	contacts = new Contact[maxContacts];
	calculateIterations = (iterations == 0);
}

World::~World()
{
	delete[] contacts;
}

unsigned World::GenerateContacts()
{
	unsigned limit = maxContacts;
	Contact *nextContact = contacts;

	for (auto i = contactGenerators.begin(); i != contactGenerators.end(); ++i)
	{
		unsigned used = (*i)->AddContact(nextContact, limit);
		limit -= used;
		nextContact += used;

		// We've run out of contacts to fill.
		// This means we're missing contacts.
		if (limit <= 0) break;
	}

	// Return the number of contacts used
	return maxContacts - limit;
}

void World::StartFrame()
{
	auto i = rigidBodies.begin();
	for (; i != rigidBodies.end(); ++i)
	{
		// Remove all forces from the accumulator
		(*i)->ClearAccumulators();
		(*i)->CalculateDerivedData();
	}
}

void World::RunPhysics(real duration)
{
	// Apply the force generators
	registry.UpdateForces(duration);

	// Integrate the objects
	for (auto i = rigidBodies.begin(); i != rigidBodies.end(); ++i)
	{
		(*i)->Integrate(duration);
	}

	// Generate contacts
	unsigned usedContacts = GenerateContacts();

	// And process them
	if (calculateIterations) resolver.SetIterations(usedContacts * 4);
	resolver.ResolveContacts(contacts, usedContacts, duration);
}