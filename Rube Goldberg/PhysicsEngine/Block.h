#ifndef BLOCK_H
#define BLOCK_H

#include "CollisionPrimitive.h"

class Block : public CollisionBox
{
public:
	Block();
	~Block();

	void SetState(const Vector3 &position, const Quaternion &orientation, const Vector3 &extents, const Vector3 &velocity);
};

#endif