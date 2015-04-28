#ifndef INTERSECTION_TESTS_H
#define INTERSECTION_TESTS_H

class CollisionBox;
class CollisionPlane;
class CollisionSphere;

class IntersectionTests
{
public:
	static bool SphereAndHalfSpace(const CollisionSphere &sphere, const CollisionPlane &plane);
	static bool SphereAndSphere(const CollisionSphere &one, const CollisionSphere &two);
	static bool BoxAndBox(const CollisionBox &one, const CollisionBox &two);
	static bool BoxAndHalfSpace(const CollisionBox &box, const CollisionPlane &plane);
};

#endif