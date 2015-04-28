#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "Vector3.h"
#include "Quaternion.h"
#include "Matrix4.h"
#include "Matrix3.h"

class RigidBody
{
public:
	RigidBody();
	~RigidBody();

	// Getters
	inline real GetMass() const { return mInverseMass == 0 ? REAL_MAX : 1 / mInverseMass; }
	inline real GetInverseMass() const { return mInverseMass; }
	inline bool HasFiniteMass() const { return mInverseMass >= 0; }

	inline real GetLinearDamping() const { return mLinearDamping; }
	inline real GetAngularDamping() const { return mAngularDamping; }

	inline Vector3 GetPosition() const { return mPosition; }
	inline Vector3 GetVelocity() const { return mVelocity; }
	inline Vector3 GetAcceleration() const { return mAcceleration; }
	inline Vector3 GetLastFrameAcceleration() const { return mLastFrameAcceleration; }

	inline Quaternion GetOrientation() const { return mOrientation; }
	inline Vector3 GetRotation() const { return mRotation; }
	
	inline Matrix3 GetInertiaTensor() const { Matrix3 it; it.SetInverse(mInverseInertiaTensor); return it; }
	inline Matrix3 GetInverseInertiaTensor() const { return mInverseInertiaTensor; }
	inline Matrix3 GetInertiaTensorWorld() const { Matrix3 it; it.SetInverse(mInverseInertiaTensorWorld); return it; }
	inline Matrix3 GetInverseInertiaTensorWorld() const { return mInverseInertiaTensorWorld; }
	inline Matrix4 GetTransform() const { return mTransformMatrix; }
	void GetGLTransform(float matrix[16]) const;

	Vector3 GetPointInLocalSpace(const Vector3 &point) const;
	Vector3 GetPointInWorldSpace(const Vector3 &point) const;
	Vector3 GetDirectionInLocalSpace(const Vector3 &direction) const;
	Vector3 GetDirectionInWorldSpace(const Vector3 &direction) const;

	inline bool GetAwake() const { return mIsAwake; }
	inline bool GetCanSleep() const { return mCanSleep; }

	// Setters
	inline void SetMass(real mass) { if (mass == 0) return; mInverseMass = 1 / mass; }
	inline void SetInverseMass(real inverseMass) { mInverseMass = inverseMass; }

	inline void SetDamping(real linear, real angular) { mLinearDamping = linear; mAngularDamping = angular; }
	inline void SetLinearDamping(real damping) { mLinearDamping = damping; }
	inline void SetAngularDamping(real damping) { mAngularDamping = damping; }

	inline void SetPosition(const Vector3 &position) { mPosition = position; }
	inline void SetPosition(real x, real y, real z) { mPosition.x = x; mPosition.y = y; mPosition.z = z; }
	inline void SetVelocity(const Vector3 &velocity) { mVelocity = velocity; }
	inline void SetVelocity(real x, real y, real z) { mVelocity.x = x; mVelocity.y = y; mVelocity.z = z; }
	inline void SetAcceleration(const Vector3 &acceleration) { mAcceleration = acceleration; }
	inline void SetAcceleration(real x, real y, real z) { mAcceleration.x = x, mAcceleration.y = y; mAcceleration.z = z; }

	inline void SetOrientation(const Quaternion &orientation) { mOrientation = orientation; }
	inline void SetOrientation(real r, real i, real j, real k) { mOrientation.r = r; mOrientation.i = i; mOrientation.j = j; mOrientation.k = k; }
	inline void SetRotation(const Vector3 &rotation) { mRotation = rotation; }
	inline void SetRotation(real x, real y, real z) { mRotation.x = x; mRotation.y = y; mRotation.z = z; }
	
	inline void SetInertiaTensor(const Matrix3 &inertiaTensor) { mInverseInertiaTensor.SetInverse(inertiaTensor); }
	inline void SetInverseInertiaTensor(const Matrix3 &inverseInertiaTensor) { mInverseInertiaTensor = inverseInertiaTensor; }

	void SetAwake(bool awake = true);
	void SetCanSleep(bool canSleep = true);

	// Functions
	void CalculateDerivedData();

	void AddVelocity(const Vector3 &deltaVelocity) { mVelocity += deltaVelocity; }
	void AddRotation(const Vector3 &deltaRotation) { mRotation += deltaRotation; }
	void AddForce(const Vector3 &force);
	void AddForceAtPoint(const Vector3 &force, const Vector3 &point);
	void AddForceAtBodyPoint(const Vector3 &force, const Vector3 &point);
	
	void AddTorque(const Vector3 &torque);

	void Integrate(real duration);

	void ClearAccumulators();

protected:
	real mInverseMass;

	real mLinearDamping;
	real mAngularDamping;

	Vector3 mPosition;
	Vector3 mVelocity;
	Vector3 mAcceleration;
	Vector3 mLastFrameAcceleration;

	Quaternion mOrientation;
	Vector3 mRotation;
	
	Vector3 mForceAccum;
	Vector3 mTorqueAccum;

	Matrix4 mTransformMatrix;
	Matrix3 mInverseInertiaTensor;
	Matrix3 mInverseInertiaTensorWorld;
	
	real mMotion;

	bool mIsAwake;
	bool mCanSleep;
};

#endif