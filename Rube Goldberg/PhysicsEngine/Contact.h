#ifndef CONTACT_H
#define CONTACT_H
#include "RigidBody.h"

class Contact
{
	friend class ContactResolver;

public:
	RigidBody *body[2];
	real friction;
	real restitution;
	Vector3 contactPoint;
	Vector3 contactNormal;
	real penetration;

	void SetBodyData(RigidBody *one, RigidBody *two, real friction, real restitution);

protected:
	Matrix3 contactToWorld;
	Vector3 contactVelocity;
	real desiredDeltaVelocity;
	Vector3 relativeContactPosition[2];

	void CalculateInternals(real duration);
	void SwapBodies();
	void MatchAwakeState();
	void CalculateDesiredDeltaVelocity(real duration);
	Vector3 CalculateLocalVelocity(unsigned bodyIndex, real duration);
	void CalculateContactBasis();
	//void ApplyImpulse(const Vector3 &impulse, RigidBody *body, Vector3 *velocityChage, Vector3 *rotationChange);
	void ApplyVelocityChange(Vector3 velocityChange[2], Vector3 rotationChange[2]);
	void ApplyPositionChange(Vector3 linearChange[2], Vector3 angularChange[2], real penetration);
	Vector3 CalculateFrictionlessImpulse(Matrix3 *inverseInertiaTensor);
	Vector3 CalculateFrictionImpulse(Matrix3 *inverseInertiaTensor);
};

#endif