#ifndef RIGID_BOX_H
#define RIGID_BOX_H

#include <PhysicsEngine/CollisionPrimitive.h>
#include <PhysicsEngine/Block.h>

#include "Model.h"

class RigidBox
{
public:
	RigidBox();
	~RigidBox();

	RigidBody* GetBody() { return box->body; }
	Block* GetBlock() { return box; }

	void SetState(const Vector3 &position, const Quaternion &orientation, const Vector3 &extents, const Vector3 &velocity) { box->SetState(position, orientation, extents, velocity); };

	void GetModelMatrix(float matrix[16]) { box->body->GetGLTransform(matrix); }
	void Draw();

private:
	Block *box;
	Model *model;
};

#endif