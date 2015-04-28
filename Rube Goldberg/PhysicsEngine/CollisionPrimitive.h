#ifndef COLLISION_PRIMITIVE_H
#define COLLISION_PRIMITIVE_H
#include "RigidBody.h"

class CollisionPrimitive
{
public:
	friend class IntersectionTests;
	friend class CollisionDetector;

	RigidBody *body;
	Matrix4 offset;

	void CalculateInternals() { transform = body->GetTransform() * offset; };

	Vector3 GetAxis(unsigned index) const { return transform.GetAxisVector(index); }
	const Matrix4& GetTransform() const { return transform; }

protected:
	Matrix4 transform;
};

class CollisionSphere : public CollisionPrimitive
{
public:
	real radius;
};

class CollisionPlane
{
public:
	Vector3 direction;
	real offset;
};

class CollisionBox : public CollisionPrimitive
{
public:
	Vector3 halfSize;
};

#endif