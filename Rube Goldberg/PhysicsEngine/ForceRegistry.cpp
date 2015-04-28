#include "ForceRegistry.h"

void ForceRegistry::Add(RigidBody* body, ForceGenerator* fg)
{
	ForceRegistration registration;
	registration.body = body;
	registration.fg = fg;
	mRegistrations.push_back(registration);
}

void ForceRegistry::Remove(RigidBody* body, ForceGenerator* fg)
{
	auto i = mRegistrations.begin();
	for (; i != mRegistrations.end(); ++i)
	{
		if (i->body == body && i->fg == fg)
		{
			mRegistrations.erase(i);
			break;
		}
	}
}

void ForceRegistry::Clear()
{
	mRegistrations.clear();
}

void ForceRegistry::UpdateForces(real duration)
{
	auto i = mRegistrations.begin();
	for (; i != mRegistrations.end(); ++i)
	{
		i->fg->UpdateForce(i->body, duration);
	}
}