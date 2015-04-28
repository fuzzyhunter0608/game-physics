#include "Joint.h"
#include "Contact.h"

void Joint::Set(RigidBody* a, const Vector3& aPos, RigidBody* b, const Vector3& bPos, real error)
{
	body[0] = a;
	body[1] = b;

	position[0] = aPos;
	position[1] = bPos;

	this->error = error;
}

unsigned Joint::AddContact(Contact* contact, unsigned limit) const
{
	// Calculate the position of each connection point in world coordinates
	Vector3 aPosWorld = body[0]->GetPointInWorldSpace(position[0]);
	Vector3 bPosWorld = body[1]->GetPointInWorldSpace(position[1]);

	// Calculate the length of the joint
	Vector3 aToB = bPosWorld - aPosWorld;
	Vector3 normal = aToB.Normalized();
	real length = aToB.Length();

	// Check if it is violated
	if (abs(length) > error)
	{
		contact->body[0] = body[0];
		contact->body[1] = body[1];
		contact->contactNormal = normal;
		contact->contactPoint = (aPosWorld + bPosWorld) * 0.5f;
		contact->penetration = length - error;
		contact->friction = 1.0f;
		contact->restitution = 0;
		return 1;
	}

	return 0;
}