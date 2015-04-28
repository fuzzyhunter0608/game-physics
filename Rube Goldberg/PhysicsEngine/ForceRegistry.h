#ifndef FORCE_REGISTRY_H
#define FORCE_REGISTRY_H

#include "RigidBody.h"
#include "ForceGenerator.h"
#include <vector>

struct ForceRegistration
{
	RigidBody *body;
	ForceGenerator *fg;
};

typedef vector<ForceRegistration> Registry;

class ForceRegistry
{
public:
	void Add(RigidBody* body, ForceGenerator *fg);
	void Remove(RigidBody* body, ForceGenerator *fg);
	void Clear();

	void UpdateForces(real duration);

private:
	Registry mRegistrations;
};

#endif