#ifndef WORLD_H
#define WORLD_H

#include "RigidBody.h"
#include "ContactResolver.h"
#include "ContactGenerator.h"
#include "ForceRegistry.h"

class World
{
public:
	typedef std::vector<RigidBody*> RigidBodies;
	typedef std::vector<ContactGenerator*> ContactGenerators;

	World(unsigned maxContacts, unsigned iterations = 0);
	~World();

	RigidBodies& GetRigidbodies() { return rigidBodies; }
	ContactGenerators& GetContactGenerators() { return contactGenerators; }
	ForceRegistry& GetForceRegistry() { return registry; }

	unsigned GenerateContacts();
	void StartFrame();
	void RunPhysics(real duration);

private:
	

	bool calculateIterations;
	RigidBodies rigidBodies;

	ForceRegistry registry;

	ContactResolver resolver;
	ContactGenerators contactGenerators;
	Contact *contacts;
	unsigned maxContacts;
};

#endif