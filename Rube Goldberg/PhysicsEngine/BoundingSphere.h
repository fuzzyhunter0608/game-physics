#ifndef BOUNDING_SPHERE_H
#define BOUNDING_SPHERE_H

#include "Vector3.h"

class BoundingSphere
{
public:
	BoundingSphere(const Vector3 &center, real radius);
	BoundingSphere(const BoundingSphere &one, const BoundingSphere &two);

	bool Overlaps(const BoundingSphere *other) const;

	real GetGrowth(const BoundingSphere &other) const;
	real GetSize() const { return ((real)1.333333) * PI * mRadius * mRadius * mRadius; }

protected:
	Vector3 mCenter;
	real mRadius;
};

#endif