#include "BoundingSphere.h"

BoundingSphere::BoundingSphere(const Vector3& center, real radius)
	:mCenter(center)
	,mRadius(radius)
{
}

BoundingSphere::BoundingSphere(const BoundingSphere& one, const BoundingSphere& two)
{
	Vector3 centerOffset = two.mCenter - one.mCenter;
	real distance = centerOffset.LengthSqr();
	real radiusDiff = two.mRadius - one.mRadius;

	// Check if the larger sphere encloses the small one
	if (radiusDiff * radiusDiff >= distance)
	{
		if (one.mRadius > two.mRadius)
		{
			mCenter = one.mCenter;
			mRadius = one.mRadius;
		}
		else
		{
			mCenter = two.mCenter;
			mRadius = two.mRadius;
		}
	}

	// Otherwise, work with partially overlapping spheres
	else
	{
		distance = sqrt(distance);
		mRadius = (distance + one.mRadius + two.mRadius) * ((real)0.5);

		// New center based on one's center, moved twoards two's center by amount proportional to the spheres' radii
		mCenter = one.mCenter;
		if (distance > 0)
		{
			mCenter += centerOffset * ((mRadius - one.mRadius) / distance);
		}
	}
}

bool BoundingSphere::Overlaps(const BoundingSphere* other) const
{
	real distanceSquared = (mCenter - other->mCenter).LengthSqr();
	return distanceSquared < (mRadius + other->mRadius) * (mRadius + other->mRadius);
}

real BoundingSphere::GetGrowth(const BoundingSphere& other) const
{
	BoundingSphere newSphere(*this, other);
	
	// Return value proportional to change in surface area
	return newSphere.mRadius * newSphere.mRadius - mRadius * mRadius;
}