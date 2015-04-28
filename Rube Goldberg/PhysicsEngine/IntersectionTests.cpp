#include "IntersectionTests.h"

#include "Core.h"
#include "CollisionPrimitive.h"

static inline bool TransformToAxis(const CollisionBox &box, const Vector3 &axis)
{
	return box.halfSize.x * abs(Vector3::Dot(axis, box.GetAxis(0))) +
		   box.halfSize.y * abs(Vector3::Dot(axis, box.GetAxis(1))) +
		   box.halfSize.z * abs(Vector3::Dot(axis, box.GetAxis(2)));
}

static inline bool OverlapOnAxis(const CollisionBox &one, const CollisionBox &two, const Vector3 &axis, const Vector3 &toCenter)
{
	// Project the half-size of one onto axis
	real oneProject = TransformToAxis(one, axis);
	real twoProject = TransformToAxis(two, axis);

	// Project this onto the axis
	real distance = abs(Vector3::Dot(toCenter, axis));

	// Check for overlap
	return (distance < oneProject + twoProject);
}

bool IntersectionTests::SphereAndHalfSpace(const CollisionSphere& sphere, const CollisionPlane& plane)
{
	// Find the distance from the origin
	real ballDistance = Vector3::Dot(plane.direction, sphere.GetAxis(3)) - sphere.radius;

	// Check for the intersection
	return ballDistance <= plane.offset;
}

bool IntersectionTests::SphereAndSphere(const CollisionSphere& one, const CollisionSphere& two)
{
	// Find the vector between the objects
	Vector3 midline = one.GetAxis(3) - two.GetAxis(3);

	// See if it is large enough
	return midline.LengthSqr() < (one.radius + two.radius) * (one.radius + two.radius);
}

bool IntersectionTests::BoxAndBox(const CollisionBox& one, const CollisionBox& two)
{
	Vector3 toCenter = two.GetAxis(3) - one.GetAxis(3);

	return (
		//Check on box one's axes first
		OverlapOnAxis(one, two, one.GetAxis(0), toCenter) &&
		OverlapOnAxis(one, two, one.GetAxis(1), toCenter) &&
		OverlapOnAxis(one, two, one.GetAxis(2), toCenter) &&

		// And on two's
		OverlapOnAxis(one, two, two.GetAxis(0), toCenter) &&
		OverlapOnAxis(one, two, two.GetAxis(1), toCenter) &&
		OverlapOnAxis(one, two, two.GetAxis(2), toCenter) &&

		// Now on the cross products
		OverlapOnAxis(one, two, Vector3::Cross(one.GetAxis(0), two.GetAxis(0)), toCenter) &&
		OverlapOnAxis(one, two, Vector3::Cross(one.GetAxis(0), two.GetAxis(1)), toCenter) &&
		OverlapOnAxis(one, two, Vector3::Cross(one.GetAxis(0), two.GetAxis(2)), toCenter) &&
		OverlapOnAxis(one, two, Vector3::Cross(one.GetAxis(1), two.GetAxis(0)), toCenter) &&
		OverlapOnAxis(one, two, Vector3::Cross(one.GetAxis(1), two.GetAxis(1)), toCenter) &&
		OverlapOnAxis(one, two, Vector3::Cross(one.GetAxis(1), two.GetAxis(2)), toCenter) &&
		OverlapOnAxis(one, two, Vector3::Cross(one.GetAxis(2), two.GetAxis(0)), toCenter) &&
		OverlapOnAxis(one, two, Vector3::Cross(one.GetAxis(2), two.GetAxis(1)), toCenter) &&
		OverlapOnAxis(one, two, Vector3::Cross(one.GetAxis(2), two.GetAxis(2)), toCenter)
		);
}

bool IntersectionTests::BoxAndHalfSpace(const CollisionBox& box, const CollisionPlane& plane)
{
	// Work out the projected radius of the box onto the plane direction
	real projectedRadius = TransformToAxis(box, plane.direction);

	// Work out how far the box is from the origin
	real boxDistance = Vector3::Dot(plane.direction, box.GetAxis(3)) - projectedRadius;

	// Check for the intersection
	return boxDistance <= plane.offset;
}