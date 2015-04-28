#include "CollisionDetector.h"
#include "Contact.h"
#include "CollisionData.h"
#include "CollisionPrimitive.h"
#include "IntersectionTests.h"

static inline real transformToAxis(const CollisionBox &box, const Vector3 &axis)
{
	return
		box.halfSize.x * abs(Vector3::Dot(axis, box.GetAxis(0))) +
		box.halfSize.y * abs(Vector3::Dot(axis, box.GetAxis(1))) +
		box.halfSize.z * abs(Vector3::Dot(axis, box.GetAxis(2)));
}

static inline bool overlapOnAxis(const CollisionBox &one, const CollisionBox &two, const Vector3 &axis, const Vector3 &toCenter)
{
	// Project the half-size of one onto axis
	real oneProject = transformToAxis(one, axis);
	real twoProject = transformToAxis(two, axis);

	// Project this onto the axis
	real distance = abs(Vector3::Dot(toCenter, axis));

	// Check for overlap
	return (distance < oneProject + twoProject);
}

static inline real penetrationOnAxis(const CollisionBox &one, const CollisionBox &two, const Vector3 &axis, const Vector3 &toCenter)
{
	// Project the half-size of one onto axis
	real oneProject = transformToAxis(one, axis);
	real twoProject = transformToAxis(two, axis);

	// Project this onto the axis
	real distance = abs(Vector3::Dot(toCenter, axis));

	// Return the overlap (i.e. positive indicates
	// overlap, negative indicates separation).
	return oneProject + twoProject - distance;
}


static inline bool tryAxis(
	const CollisionBox &one,
	const CollisionBox &two,
	Vector3 axis,
	const Vector3& toCentre,
	unsigned index,

	// These values may be updated
	real& smallestPenetration,
	unsigned &smallestCase
	)
{
	// Make sure we have a normalized axis, and don't check almost parallel axes
	if (axis.LengthSqr() < 0.0001) return true;
	axis.Normalize();

	real penetration = penetrationOnAxis(one, two, axis, toCentre);

	if (penetration < 0) return false;
	if (penetration < smallestPenetration) {
		smallestPenetration = penetration;
		smallestCase = index;
	}
	return true;
}

void fillPointFaceBoxBox(
	const CollisionBox &one,
	const CollisionBox &two,
	const Vector3 &toCentre,
	CollisionData *data,
	unsigned best,
	real pen
	)
{
	// This method is called when we know that a vertex from
	// box two is in contact with box one.

	Contact* contact = data->contacts;

	// We know which axis the collision is on (i.e. best),
	// but we need to work out which of the two faces on
	// this axis.
	Vector3 normal = one.GetAxis(best);
	if (Vector3::Dot(one.GetAxis(best), toCentre) > 0)
	{
		normal = normal * -1.0f;
	}

	// Work out which vertex of box two we're colliding with.
	// Using toCentre doesn't work!
	Vector3 vertex = two.halfSize;
	if (Vector3::Dot(two.GetAxis(0), normal) < 0) vertex.x = -vertex.x;
	if (Vector3::Dot(two.GetAxis(1), normal) < 0) vertex.y = -vertex.y;
	if (Vector3::Dot(two.GetAxis(2), normal) < 0) vertex.z = -vertex.z;

	// Create the contact data
	contact->contactNormal = normal;
	contact->penetration = pen;
	contact->contactPoint = two.GetTransform() * vertex;
	contact->SetBodyData(one.body, two.body,
		data->friction, data->restitution);
}

static inline Vector3 contactPoint(
	const Vector3 &pOne,
	const Vector3 &dOne,
	real oneSize,
	const Vector3 &pTwo,
	const Vector3 &dTwo,
	real twoSize,

	// If this is true, and the contact point is outside
	// the edge (in the case of an edge-face contact) then
	// we use one's midpoint, otherwise we use two's.
	bool useOne)
{
	Vector3 toSt, cOne, cTwo;
	real dpStaOne, dpStaTwo, dpOneTwo, smOne, smTwo;
	real denom, mua, mub;

	smOne = dOne.LengthSqr();
	smTwo = dTwo.LengthSqr();
	dpOneTwo = Vector3::Dot(dTwo, dOne);

	toSt = pOne - pTwo;
	dpStaOne = Vector3::Dot(dOne, toSt);
	dpStaTwo = Vector3::Dot(dTwo, toSt);

	denom = smOne * smTwo - dpOneTwo * dpOneTwo;

	// Zero denominator indicates parrallel lines
	if (abs(denom) < 0.0001f) {
		return useOne ? pOne : pTwo;
	}

	mua = (dpOneTwo * dpStaTwo - smTwo * dpStaOne) / denom;
	mub = (smOne * dpStaTwo - dpOneTwo * dpStaOne) / denom;

	// If either of the edges has the nearest point out
	// of bounds, then the edges aren't crossed, we have
	// an edge-face contact. Our point is on the edge, which
	// we know from the useOne parameter.
	if (mua > oneSize ||
		mua < -oneSize ||
		mub > twoSize ||
		mub < -twoSize)
	{
		return useOne ? pOne : pTwo;
	}
	else
	{
		cOne = pOne + dOne * mua;
		cTwo = pTwo + dTwo * mub;

		return cOne * 0.5 + cTwo * 0.5;
	}
}

unsigned CollisionDetector::SphereAndHalfSpace(const CollisionSphere& sphere, const CollisionPlane& plane, CollisionData* data)
{
	// Make sure we have contacts
	if (data->contactsLeft <= 0) return 0;

	// Cache the sphere position
	Vector3 position = sphere.GetAxis(3);

	// Find the distance from the plane
	real centreDistance = Vector3::Dot(plane.direction, position) - plane.offset;

	// Check if we're within radius
	if (centreDistance*centreDistance > sphere.radius*sphere.radius)
	{
		return 0;
	}

	// Check which side of the plane we're on
	Vector3 normal = plane.direction;
	real penetration = -centreDistance;
	if (centreDistance < 0)
	{
		normal *= -1;
		penetration = -penetration;
	}
	penetration += sphere.radius;

	// Create the contact - it has a normal in the plane direction.
	Contact* contact = data->contacts;
	contact->contactNormal = normal;
	contact->penetration = penetration;
	contact->contactPoint = position - plane.direction * centreDistance;
	contact->SetBodyData(sphere.body, NULL,
		data->friction, data->restitution);

	data->AddContacts(1);
	return 1;
}

unsigned CollisionDetector::SphereAndTruePlane(const CollisionSphere& sphere, const CollisionPlane& plane, CollisionData* data)
{
	// Make sure we have contacts
	if (data->contactsLeft <= 0) return 0;

	// Cache the sphere position
	Vector3 position = sphere.GetAxis(3);

	// Find the distance from the plane
	real centreDistance = Vector3::Dot(plane.direction, position) - plane.offset;

	// Check if we're within radius
	if (centreDistance*centreDistance > sphere.radius*sphere.radius)
	{
		return 0;
	}

	// Check which side of the plane we're on
	Vector3 normal = plane.direction;
	real penetration = -centreDistance;
	if (centreDistance < 0)
	{
		normal *= -1;
		penetration = -penetration;
	}
	penetration += sphere.radius;

	// Create the contact - it has a normal in the plane direction.
	Contact* contact = data->contacts;
	contact->contactNormal = normal;
	contact->penetration = penetration;
	contact->contactPoint = position - plane.direction * centreDistance;
	contact->SetBodyData(sphere.body, NULL, data->friction, data->restitution);

	data->AddContacts(1);
	return 1;
}

unsigned CollisionDetector::SphereAndSphere(const CollisionSphere& one, const CollisionSphere& two, CollisionData* data)
{
	// Make sure we have contacts
	if (data->contactsLeft <= 0) return 0;

	// Cache the sphere positions
	Vector3 positionOne = one.GetAxis(3);
	Vector3 positionTwo = two.GetAxis(3);

	// Find the vector between the objects
	Vector3 midline = positionOne - positionTwo;
	real size = midline.Length();

	// See if it is large enough.
	if (size <= 0.0f || size >= one.radius + two.radius)
	{
		return 0;
	}

	// We manually create the normal, because we have the
	// size to hand.
	Vector3 normal = midline * (((real)1.0) / size);

	Contact* contact = data->contacts;
	contact->contactNormal = normal;
	contact->contactPoint = positionOne + midline * (real)0.5;
	contact->penetration = (one.radius + two.radius - size);
	contact->SetBodyData(one.body, two.body,
		data->friction, data->restitution);

	data->AddContacts(1);
	return 1;
}

unsigned CollisionDetector::BoxAndHalfSpace(const CollisionBox& box, const CollisionPlane& plane, CollisionData* data)
{
	// Make sure we have contacts
	if (data->contactsLeft <= 0) return 0;

	// Check for intersection
	if (!IntersectionTests::BoxAndHalfSpace(box, plane))
	{
		return 0;
	}

	// We have an intersection, so find the intersection points. We can make
	// do with only checking vertices. If the box is resting on a plane
	// or on an edge, it will be reported as four or two contact points.

	// Go through each combination of + and - for each half-size
	static real mults[8][3] = { { 1, 1, 1 }, { -1, 1, 1 }, { 1, -1, 1 }, { -1, -1, 1 },
	{ 1, 1, -1 }, { -1, 1, -1 }, { 1, -1, -1 }, { -1, -1, -1 } };

	Contact* contact = data->contacts;
	unsigned contactsUsed = 0;
	for (unsigned i = 0; i < 8; i++) {

		// Calculate the position of each vertex
		Vector3 vertexPos(mults[i][0], mults[i][1], mults[i][2]);
		vertexPos.x *= box.halfSize.x;
		vertexPos.y *= box.halfSize.y;
		vertexPos.z *= box.halfSize.z;
		vertexPos = box.transform.Transform(vertexPos);

		// Calculate the distance from the plane
		real vertexDistance = Vector3::Dot(vertexPos, plane.direction);

		// Compare this to the plane's distance
		if (vertexDistance <= plane.offset)
		{
			// Create the contact data.

			// The contact point is halfway between the vertex and the
			// plane - we multiply the direction by half the separation
			// distance and add the vertex location.
			contact->contactPoint = plane.direction;
			contact->contactPoint *= (vertexDistance - plane.offset);
			contact->contactPoint += vertexPos;
			contact->contactNormal = plane.direction;
			contact->penetration = plane.offset - vertexDistance;

			// Write the appropriate data
			contact->SetBodyData(box.body, NULL,
				data->friction, data->restitution);

			// Move onto the next contact
			contact++;
			contactsUsed++;
			if (contactsUsed == (unsigned)data->contactsLeft) return contactsUsed;
		}
	}

	data->AddContacts(contactsUsed);
	return contactsUsed;
}

unsigned CollisionDetector::BoxAndBox(const CollisionBox& one, const CollisionBox& two, CollisionData* data)
{
	// Find the vector between the two centres
	Vector3 toCentre = two.GetAxis(3) - one.GetAxis(3);

	// We start assuming there is no contact
	real pen = REAL_MAX;
	unsigned best = 0xffffff;

	// Now we check each axes, returning if it gives us
	// a separating axis, and keeping track of the axis with
	// the smallest penetration otherwise.
	if (!tryAxis(one, two, (one.GetAxis(0)), toCentre, (0), pen, best)) return 0;
	if (!tryAxis(one, two, (one.GetAxis(1)), toCentre, (1), pen, best)) return 0;
	if (!tryAxis(one, two, (one.GetAxis(2)), toCentre, (2), pen, best)) return 0;
					  
	if (!tryAxis(one, two, (two.GetAxis(0)), toCentre, (3), pen, best)) return 0;
	if (!tryAxis(one, two, (two.GetAxis(1)), toCentre, (4), pen, best)) return 0;
	if (!tryAxis(one, two, (two.GetAxis(2)), toCentre, (5), pen, best)) return 0;

	// Store the best axis-major, in case we run into almost
	// parallel edge collisions later
	unsigned bestSingleAxis = best;

	if (!tryAxis(one, two, one.GetAxis(0), toCentre, (6), pen, best)) return 0; 
	if (!tryAxis(one, two, one.GetAxis(0), toCentre, (7), pen, best)) return 0; 
	if (!tryAxis(one, two, one.GetAxis(0), toCentre, (8), pen, best)) return 0; 
	if (!tryAxis(one, two, one.GetAxis(1), toCentre, (9), pen, best)) return 0; 
	if (!tryAxis(one, two, one.GetAxis(1), toCentre, (10), pen, best)) return 0;
	if (!tryAxis(one, two, one.GetAxis(1), toCentre, (11), pen, best)) return 0;
	if (!tryAxis(one, two, one.GetAxis(2), toCentre, (12), pen, best)) return 0;
	if (!tryAxis(one, two, one.GetAxis(2), toCentre, (13), pen, best)) return 0;
	if (!tryAxis(one, two, one.GetAxis(2), toCentre, (14), pen, best)) return 0;

	// We now know there's a collision, and we know which
	// of the axes gave the smallest penetration. We now
	// can deal with it in different ways depending on
	// the case.
	if (best < 3)
	{
		// We've got a vertex of box two on a face of box one.
		fillPointFaceBoxBox(one, two, toCentre, data, best, pen);
		data->AddContacts(1);
		return 1;
	}
	else if (best < 6)
	{
		// We've got a vertex of box one on a face of box two.
		// We use the same algorithm as above, but swap around
		// one and two (and therefore also the vector between their
		// centres).
		fillPointFaceBoxBox(two, one, toCentre*-1.0f, data, best - 3, pen);
		data->AddContacts(1);
		return 1;
	}
	else
	{
		// We've got an edge-edge contact. Find out which axes
		best -= 6;
		unsigned oneAxisIndex = best / 3;
		unsigned twoAxisIndex = best % 3;
		Vector3 oneAxis = one.GetAxis(oneAxisIndex);
		Vector3 twoAxis = two.GetAxis(twoAxisIndex);
		Vector3 axis = Vector3::Cross(oneAxis, twoAxis);
		axis.Normalize();

		// The axis should point from box one to box two.
		if (Vector3::Dot(axis, toCentre) > 0) axis = axis * -1.0f;

		// We have the axes, but not the edges: each axis has 4 edges parallel
		// to it, we need to find which of the 4 for each object. We do
		// that by finding the point in the centre of the edge. We know
		// its component in the direction of the box's collision axis is zero
		// (its a mid-point) and we determine which of the extremes in each
		// of the other axes is closest.
		Vector3 ptOnOneEdge = one.halfSize;
		Vector3 ptOnTwoEdge = two.halfSize;
		for (unsigned i = 0; i < 3; i++)
		{
			if (i == oneAxisIndex) ptOnOneEdge[i] = 0;
			else if (Vector3::Dot(one.GetAxis(i), axis) > 0) ptOnOneEdge[i] = -ptOnOneEdge[i];

			if (i == twoAxisIndex) ptOnTwoEdge[i] = 0;
			else if (Vector3::Dot(two.GetAxis(i), axis) < 0) ptOnTwoEdge[i] = -ptOnTwoEdge[i];
		}

		// Move them into world coordinates (they are already oriented
		// correctly, since they have been derived from the axes).
		ptOnOneEdge = one.transform * ptOnOneEdge;
		ptOnTwoEdge = two.transform * ptOnTwoEdge;

		// So we have a point and a direction for the colliding edges.
		// We need to find out point of closest approach of the two
		// line-segments.
		Vector3 vertex = contactPoint(
			ptOnOneEdge, oneAxis, one.halfSize[oneAxisIndex],
			ptOnTwoEdge, twoAxis, two.halfSize[twoAxisIndex],
			bestSingleAxis > 2
			);

		// We can fill the contact.
		Contact* contact = data->contacts;

		contact->penetration = pen;
		contact->contactNormal = axis;
		contact->contactPoint = vertex;
		contact->SetBodyData(one.body, two.body,
			data->friction, data->restitution);
		data->AddContacts(1);
		return 1;
	}
	return 0;
}

unsigned CollisionDetector::BoxAndPoint(const CollisionBox& box, const Vector3& point, CollisionData* data)
{
	// Transform the point into box coordinates
	Vector3 relPt = box.transform.TransformInverse(point);

	Vector3 normal;

	// Check each axis, looking for the axis on which the
	// penetration is least deep.
	real min_depth = box.halfSize.x - abs(relPt.x);
	if (min_depth < 0) return 0;
	normal = box.GetAxis(0) * ((relPt.x < 0) ? -1 : 1);

	real depth = box.halfSize.y - abs(relPt.y);
	if (depth < 0) return 0;
	else if (depth < min_depth)
	{
		min_depth = depth;
		normal = box.GetAxis(1) * ((relPt.y < 0) ? -1 : 1);
	}

	depth = box.halfSize.z - abs(relPt.z);
	if (depth < 0) return 0;
	else if (depth < min_depth)
	{
		min_depth = depth;
		normal = box.GetAxis(2) * ((relPt.z < 0) ? -1 : 1);
	}

	// Compile the contact
	Contact* contact = data->contacts;
	contact->contactNormal = normal;
	contact->contactPoint = point;
	contact->penetration = min_depth;

	// Note that we don't know what rigid body the point
	// belongs to, so we just use NULL. Where this is called
	// this value can be left, or filled in.
	contact->SetBodyData(box.body, NULL,
		data->friction, data->restitution);

	data->AddContacts(1);
	return 1;
}

unsigned CollisionDetector::BoxAndSphere(const CollisionBox& box, const CollisionSphere& sphere, CollisionData* data)
{
	// Transform the centre of the sphere into box coordinates
	Vector3 centre = sphere.GetAxis(3);
	Vector3 relCentre = box.transform.TransformInverse(centre);

	// Early out check to see if we can exclude the contact
	if (abs(relCentre.x) - sphere.radius > box.halfSize.x ||
		abs(relCentre.y) - sphere.radius > box.halfSize.y ||
		abs(relCentre.z) - sphere.radius > box.halfSize.z)
	{
		return 0;
	}

	Vector3 closestPt(0, 0, 0);
	real dist;

	// Clamp each coordinate to the box.
	dist = relCentre.x;
	if (dist > box.halfSize.x) dist = box.halfSize.x;
	if (dist < -box.halfSize.x) dist = -box.halfSize.x;
	closestPt.x = dist;

	dist = relCentre.y;
	if (dist > box.halfSize.y) dist = box.halfSize.y;
	if (dist < -box.halfSize.y) dist = -box.halfSize.y;
	closestPt.y = dist;

	dist = relCentre.z;
	if (dist > box.halfSize.z) dist = box.halfSize.z;
	if (dist < -box.halfSize.z) dist = -box.halfSize.z;
	closestPt.z = dist;

	// Check we're in contact
	dist = (closestPt - relCentre).LengthSqr();
	if (dist > sphere.radius * sphere.radius) return 0;

	// Compile the contact
	Vector3 closestPtWorld = box.transform.Transform(closestPt);

	Contact* contact = data->contacts;
	contact->contactNormal = (closestPtWorld - centre);
	contact->contactNormal.Normalize();
	contact->contactPoint = closestPtWorld;
	contact->penetration = sphere.radius - sqrt(dist);
	contact->SetBodyData(box.body, sphere.body,
		data->friction, data->restitution);

	data->AddContacts(1);
	return 1;
}