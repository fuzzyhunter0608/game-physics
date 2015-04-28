#ifndef COLLISION_DETECTOR_H
#define COLLISION_DETECTOR_H

class Vector3;
class CollisionBox;
class CollisionPlane;
struct CollisionData;
class CollisionSphere;

class CollisionDetector
{
public:
	static unsigned SphereAndHalfSpace(const CollisionSphere &sphere, const CollisionPlane &plane, CollisionData *data);
	static unsigned SphereAndTruePlane(const CollisionSphere &sphere, const CollisionPlane &plane, CollisionData *data);
	static unsigned SphereAndSphere(const CollisionSphere &one, const CollisionSphere &two, CollisionData *data);
	static unsigned BoxAndHalfSpace(const CollisionBox &box, const CollisionPlane &plane, CollisionData *data);
	static unsigned BoxAndBox(const CollisionBox &one, const CollisionBox &two, CollisionData *data);
	static unsigned BoxAndPoint(const CollisionBox &box, const Vector3 &point, CollisionData *data);
	static unsigned BoxAndSphere(const CollisionBox &box, const CollisionSphere &sphere, CollisionData *data);
};

#endif