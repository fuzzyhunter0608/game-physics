#include "Block.h"

Block::Block()
{
	body = new RigidBody();
}

Block::~Block()
{
	delete body;
}

void Block::SetState(const Vector3& position, const Quaternion& orientation, const Vector3& extents, const Vector3& velocity)
{
	body->SetPosition(position);
	body->SetOrientation(orientation);
	body->SetVelocity(velocity);
	body->SetRotation(Vector3::zero);
	halfSize = extents;

	real mass = 1;
	body->SetMass(mass);

	Matrix3 tensor;
	tensor.SetBlockInertiaTensor(halfSize, mass);
	body->SetInertiaTensor(tensor);

	body->SetLinearDamping(0.99f);
	body->SetAngularDamping(0.99f);
	body->ClearAccumulators();

	body->SetAwake();
	body->CalculateDerivedData();
}